//
//  HitView.swift
//  MindGuard
//
//  Created by Richard Henry on 28/03/2023.
//

import SwiftUI

struct HitView: View {
    @StateObject private var viewModel = ViewModel()
    
    var body: some View {
        NavigationStack {
            List {
                ForEach(viewModel.coins) { hit in
                    NavigationLink(value: hit) {
                        
                        HitCell(coin: hit)
                    }
                }
            }
            .navigationTitle("Hits")
            .listStyle(.grouped)
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
        
        let BASE_URL = "https://api.coingecko.com/api/v3/coins/"
        var urlString: String { "\(BASE_URL)markets?vs_currency=usd&order=market_cap_desc&per_page=50&page=1&price_change_percentage=24h" }
        
        init() {
            fetchCoinsWithURLSession()
        }
        
        func fetchCoinsWithURLSession() {
            guard let url = URL(string: urlString) else {
                print("DEBUG: Invalid URL")
                return
            }
            
            URLSession.shared.dataTask(with: url) { [weak self] data, response, error in
                guard let self = self else { return }
                
                DispatchQueue.main.async {
                    if let error = error {
                        print("DEBUG: Error \(error)")
                        return
                    }
                    
                    guard (response as? HTTPURLResponse)?.statusCode == 200 else {
                        print("DEBUG: Server error")
                        return
                    }
                    
                    guard let data = data else {
                        print("DEBUG: Invalid data")
                        return
                    }
                    
                    guard let coins = try? JSONDecoder().decode([Coin].self, from: data) else {
                        print("DEBUG: Invalid data")
                        return
                    }
                    
                    self.coins = coins
                }
            }.resume()
        }
    }
}
