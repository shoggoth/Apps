//
//  DogstarApp.swift
//  Dogstar
//
//  Created by Richard Henry on 22/03/2023.
//

import SwiftUI

@main
struct DogstarApp: App {
    @StateObject var order = Order()
    
    var body: some Scene {
        WindowGroup {
            MainView()
                .environmentObject(order)
        }
    }
}

// MARK: - Helper

extension Bundle {
    
    func decode<T: Decodable>(_ type: T.Type, from file: String) -> T {
        guard let url = self.url(forResource: file, withExtension: nil) else { fatalError("Failed to locate \(file) in bundle.") }
        guard let data = try? Data(contentsOf: url) else { fatalError("Failed to load \(file) from bundle.") }

        let decoder = JSONDecoder()

        guard let loaded = try? decoder.decode(T.self, from: data) else { fatalError("Failed to decode \(file) from bundle.") }

        return loaded
    }
}
