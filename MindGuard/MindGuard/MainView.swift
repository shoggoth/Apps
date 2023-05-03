//
//  MainView.swift
//  MindGuard
//
//  Created by Richard Henry on 28/03/2023.
//

import SwiftUI

struct MainView: View {
    private enum Tag {
        case map
        case depsych
        case chalk
        case hitList
        case psIdent
    }
    @State private var selectedTab = Tag.psIdent
    
    var body: some View {
        TabView(selection: $selectedTab) {
            HitMapView()
                .tabItem { Label("HitMap", systemImage: "list.dash") }
                .tag(Tag.map)
            DepsychView()
                .tabItem { Label("Depsych", systemImage: "list.dash") }
                .tag(Tag.depsych)
            ChalkView()
                .tabItem { Label("PsyChalk", systemImage: "list.dash") }
                .tag(Tag.chalk)
            HitListView()
                .tabItem { Label("Hits", systemImage: "list.dash") }
                .tag(Tag.hitList)
            PsIdentView()
                .tabItem { Label("PsIdent", systemImage: "list.dash") }
                .tag(Tag.psIdent)
        }
    }
}

struct MainView_Previews: PreviewProvider {
    static var previews: some View {
        MainView()
    }
}
