//
//  ChalkView.swift
//  MindGuard
//
//  Created by Richard Henry on 29/03/2023.
//

import SwiftUI
import RealityKit

struct ChalkView: View {
    var body: some View {
        ChalkViewContainer()
            .ignoresSafeArea(.all, edges: [.top, .trailing, .leading])
    }
}

private struct ChalkViewContainer: UIViewRepresentable {
    
    func makeUIView(context: Context) -> ARView {
        
        let arView = ARView(frame: .zero)
        
        // Load the "Box" scene from the "Cube" Reality File
        //let boxAnchor = try! Cube.loadBox()
        
        // Add the box anchor to the scene
        //arView.scene.anchors.append(boxAnchor)
        
        return arView
    }
    
    func updateUIView(_ uiView: ARView, context: Context) {}
}

struct ChalkView_Previews: PreviewProvider {
    static var previews: some View {
        ChalkView()
    }
}
