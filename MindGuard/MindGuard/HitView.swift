//
//  HitView.swift
//  MindGuard
//
//  Created by Richard Henry on 28/03/2023.
//

import SwiftUI

struct HitView: View {
    @StateObject private var viewModel = ViewModel()
    @State private var alertShown = false
    
    var body: some View {
        NavigationStack {
            List {
                ForEach(viewModel.coins) { hit in
                    NavigationLink(value: hit) {
                        HitCell(coin: hit)
                            .onAppear {
                                if hit == viewModel.coins.last { viewModel.fetchNextPage() }
                            }
                    }
                }
            }
            .navigationTitle("Hits")
            .listStyle(.grouped)
            .refreshable { viewModel.refresh() }
            .onReceive(viewModel.$error) { error in alertShown = (error != nil) }
            .alert(isPresented: $alertShown) {
                Alert(title: Text("Error"), message: Text(viewModel.error?.localizedDescription ?? "Unknown"))
            }
        }
    }
}

struct HitView_Previews: PreviewProvider {
    static var previews: some View {
        HitView()
    }
}

// MARK: - ViewModel

extension HitView {
    @MainActor class ViewModel: ObservableObject {
        @Published private(set) var coins = [Coin]()
        @Published private(set) var error: Error?
        
        private let pageSize = 20
        private var page = 0
        
        let BASE_URL = "https://api.coingecko.com/api/v3/coins/"
        var urlString: String { "\(BASE_URL)markets?vs_currency=gbp&order=market_cap_desc&per_page=\(pageSize)&page=\(page)&price_change_percentage=24h" }
        
        init() {
            fetchNextPage()
        }
        
        func fetchNextPage() {
            page += 1; print("fetch \(page)")
            Task(priority: .medium) {
                try await fetchCoins()
            }
        }
        
        func refresh() {
            page = 0
            coins.removeAll()
            fetchNextPage()
        }
        
        private func fetchCoins() async throws {
            do {
                guard let url = URL(string: urlString) else { throw Coin.FetchError.invalidURL}
                let (data, response) = try await URLSession.shared.data(from: url)
                guard (response as? HTTPURLResponse)?.statusCode == 200 else { throw Coin.FetchError.serverError }
                guard let coins = try? JSONDecoder().decode([Coin].self, from: data) else { throw Coin.FetchError.invalidData }
                self.coins.append(contentsOf: coins)
            } catch {
                self.error = error
            }
        }
    }
}
