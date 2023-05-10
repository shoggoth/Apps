//
//  HitListView.swift
//  MindGuard
//
//  Created by Richard Henry on 28/03/2023.
//

import SwiftUI

struct HitListView: View {
    @FetchRequest(sortDescriptors: [NSSortDescriptor(keyPath: \Hit.date, ascending: false)]) private var hits: FetchedResults<Hit>
    @Environment(\.managedObjectContext) private var moc

    var body: some View {
        VStack {
            List(hits) { hit in
                Text(hit.name ?? "Unknown Hit")
            }
            Button("Add") {
                let firstNames = ["Ginny", "Harry", "Hermione", "Luna", "Ron"]
                let lastNames = ["Granger", "Lovegood", "Potter", "Weasley"]

                let chosenFirstName = firstNames.randomElement()!
                let chosenLastName = lastNames.randomElement()!

                let hit = Hit(context: moc)
                hit.id = UUID()
                hit.name = "\(chosenFirstName) \(chosenLastName)"
                hit.date = Date()
                
                withAnimation { try? moc.save() }
            }
            .padding()
            Text("moc: \(moc.hashValue)")
                .foregroundColor(.red)
        }
    }
}

struct HitView_Previews: PreviewProvider {
    static var previews: some View {
        Group {
            HitListView()
                .navigationViewStyle(StackNavigationViewStyle())
                .environment(\.colorScheme, .light)
                .environment(\.managedObjectContext, HitDataController.shared.container.viewContext)
                .previewDisplayName("Light")
            
            HitListView()
                .navigationViewStyle(StackNavigationViewStyle())
                .environment(\.colorScheme, .dark)
                .environment(\.managedObjectContext, HitDataController.shared.container.viewContext)
                .previewDisplayName("Dark")
        }
    }
}
