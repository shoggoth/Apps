//
//  MenuView.swift
//  Dogstar
//
//  Created by Richard Henry on 22/03/2023.
//

import SwiftUI

struct MenuView: View {
    
    let menu = Bundle.main.decode([MenuSection].self, from: "menu.json")
    
    var body: some View {
        NavigationStack {
            List {
                ForEach(menu) { section in
                    Section(section.name) {
                        ForEach(section.items) { item in
                            NavigationLink(value: item) {
                                ItemRow(item: item)
                            }
                        }
                    }
                }
            }
            .navigationTitle("Menu")
            .listStyle(.grouped)            // The default is the insetGrouped style
            .navigationDestination(for: MenuItem.self) { item in ItemDetail(item: item) }
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        MenuView()
    }
}
