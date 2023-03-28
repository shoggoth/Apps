//
//  MainView.swift
//  MindGuard
//
//  Created by Richard Henry on 28/03/2023.
//

import SwiftUI

struct MainView: View {
    var body: some View {
        TabView {
            HitView()
              .tabItem { Label("Hits", systemImage: "list.dash") }
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
       MainView()
    }
}
