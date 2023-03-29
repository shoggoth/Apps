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
            DepsychView()
                .tabItem { Label("Depsych", systemImage: "list.dash") }
            ChalkView()
                .tabItem { Label("PsyChalk", systemImage: "list.dash") }
            HitView()
                .tabItem { Label("Hits", systemImage: "list.dash") }
        }
    }
}

struct MainView_Previews: PreviewProvider {
    static var previews: some View {
        MainView()
    }
}
