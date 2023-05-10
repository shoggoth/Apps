//
//  HitDataController.swift
//  MindGuard
//
//  Created by Richard Henry on 10/05/2023.
//

import CoreData

struct HitDataController {
    static let shared = HitDataController()
    let container = NSPersistentCloudKitContainer(name: "Hit")
    
    init() {
        container.loadPersistentStores() { _, error in
            if let error = error { fatalError("Hit store error: \(error.localizedDescription)") }
        }
        container.viewContext.automaticallyMergesChangesFromParent = true
        container.viewContext.mergePolicy = NSMergeByPropertyObjectTrumpMergePolicy
    }
}
