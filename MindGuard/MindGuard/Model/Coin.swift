//
//  Coin.swift
//  CryptoAsyncAwait
//
//  Created by Stephan Dowless on 1/5/23.
//

import Foundation

struct Coin: Codable, Identifiable, Hashable {
    let id, symbol, name: String
    let image: String
    let currentPrice: Double
    let marketCapRank: Int
    let priceChange24H, priceChangePercentage24H: Double
    
    var imageUrl: URL? {
        return URL(string: image)
    }
    
    enum CodingKeys: String, CodingKey {
        case id, symbol, name, image
        case currentPrice = "current_price"
        case marketCapRank = "market_cap_rank"
        case priceChange24H = "price_change_24h"
        case priceChangePercentage24H = "price_change_percentage_24h"
    }
    
    enum CoinError: Error, LocalizedError {
        case invalidURL
        case serverError
        case invalidData
        case unknown(Error)
        
        var errorDescription: String? {
            switch self {
            case .invalidURL:
                return ""
            case .serverError:
                return "There was an error with the server. Please try again later"
            case .invalidData:
                return "The coin data is invalid. Please try again later"
            case .unknown(let error):
                return error.localizedDescription
            }
        }
    }
}

extension Coin {
    static var sample = Coin(
        id: "Bitcoin", symbol: "BTC",
        name: "Bitcoin",
        image: "",
        currentPrice: 16700,
        marketCapRank: 1,
        priceChange24H: 200,
        priceChangePercentage24H: 2.0
    )
}
