//
//  DepsychView.swift
//  MindGuard
//
//  Created by Richard Henry on 29/03/2023.
//

import SwiftUI
import SceneKit

struct DepsychView: View {
    private var scene: SCNScene? {
        SCNScene(named: "DePsych.scnassets/ship.scn")
    }

    var cameraNode: SCNNode? {
        let cameraNode = SCNNode()
        cameraNode.camera = SCNCamera()
        cameraNode.position = SCNVector3(x: 0, y: 0, z: 2)
        return cameraNode
    }

    var body: some View {
        SceneView(
            scene: scene,
            pointOfView: cameraNode,
            options: [
                .allowsCameraControl,
                .autoenablesDefaultLighting,
                .temporalAntialiasingEnabled
            ]
        )
        .ignoresSafeArea(.all, edges: [.top, .trailing, .leading])
    }
}

struct DepsychView_Previews: PreviewProvider {
    static var previews: some View {
        DepsychView()
    }
}
