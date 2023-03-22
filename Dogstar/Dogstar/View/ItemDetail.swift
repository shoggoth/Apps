//
//  ItemDetail.swift
//  Dogstar
//
//  Created by Richard Henry on 22/03/2023.
//

import SwiftUI

struct ItemDetail: View {
    let item: MenuItem
    
    var body: some View {
        VStack {
            ZStack(alignment:.bottomTrailing) {
                Image(item.mainImage)
                    .resizable()
                    .scaledToFit()
                
                Text("Photo: \(item.photoCredit)")
                    .padding(3)
                    .background(Color(white: 0, opacity: 0.5))
                    .cornerRadius(3)
                    .font(.caption)
                    .foregroundColor(.white)
                    .offset(x: -3, y: -3)
            }
            
            Text(item.description)
                .padding()
            
            Spacer()
        }
        .navigationTitle(item.name)
        .navigationBarTitleDisplayMode(.inline)     // Apple recomends only to use the full size title on the top level.
    }
}

struct ItemDetail_Previews: PreviewProvider {
    static var previews: some View {
        NavigationStack {
            ItemDetail(item: MenuItem.example)
        }
    }
}
