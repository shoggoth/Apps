//
//  PsIdentView.swift
//  MindGuard
//
//  Created by Richard Henry on 01/04/2023.
//

import SwiftUI

struct PsIdentView: View {
    var body: some View {
        VStack(spacing: 23) {
            Text("Biorythm and Eponymlog go here.")
            Button("Vanilla Button") {
                print("pressed")
            }
            Button("Capsule Button") {
                print("pressed")
            }
            .padding()
            .background(Color(red: 0, green: 0, blue: 0.5))
            .clipShape(Capsule())
            Button("Capsule Button") {
                print("pressed")
            }
            .buttonStyle(BlueButton())
            Button("Capsule Button") {
                print("pressed")
            }
            .buttonStyle(GrowingButton())
        }
    }
}

struct PsIdentView_Previews: PreviewProvider {
    static var previews: some View {
        PsIdentView()
    }
}

struct BlueButton: ButtonStyle {
    func makeBody(configuration: Configuration) -> some View {
        configuration.label
            .padding()
            .background(Color(red: 0, green: 0, blue: 0.5))
            .foregroundColor(.accentColor)
            .clipShape(Capsule())
    }
}

struct GrowingButton: ButtonStyle {
    func makeBody(configuration: Configuration) -> some View {
        configuration.label
            .padding()
            .background(Color(red: 0, green: 0, blue: 0.5))
            .foregroundColor(.accentColor)
            .clipShape(Capsule())
            .scaleEffect(configuration.isPressed ? 1.2 : 1)
            .animation(.easeOut(duration: 0.2), value: configuration.isPressed)
    }
}
