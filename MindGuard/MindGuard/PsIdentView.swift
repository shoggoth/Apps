//
//  PsIdentView.swift
//  MindGuard
//
//  Created by Richard Henry on 01/04/2023.
//

import SwiftUI

struct PsIdentView: View {
    @State private var sliderValue = 0.5
    
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
            .buttonStyle(GreyButton())
            Button("Capsule Button") {
                print("pressed")
            }
            .buttonStyle(GrowingButton())
            VStack(spacing: 20) {
                Button("Button 1") {
                }.accentColor(nil)
                
                Button("Button 2") {
                    
                }.accentColor(.green)
                
                Slider(value: $sliderValue)
            }.accentColor(.pink)
        }
    }
}

struct PsIdentView_Previews: PreviewProvider {
    static var previews: some View {
        PsIdentView()
    }
}

struct GreyButton: ButtonStyle {
    @Environment(\.colorScheme) var colorScheme

    func makeBody(configuration: Configuration) -> some View {
        configuration.label
            .padding()
            .background(Color(red: 0.5, green: 0.5, blue: 0.5))
            .foregroundColor(colorScheme == .dark ? .white : .black)
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
