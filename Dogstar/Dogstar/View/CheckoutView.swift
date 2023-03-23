//
//  CheckoutView.swift
//  Dogstar
//
//  Created by Richard Henry on 23/03/2023.
//

import SwiftUI

struct CheckoutView: View {
    @EnvironmentObject var order: Order
    
    let paymentTypes = ["Cash", "Credit Card", "Points"]
    @State private var paymentType = "Cash"
    
    @State private var addLoyaltyDetails = false
    @State private var loyaltyNumber = ""
    
    @State private var paymentAlertShowing = false

    let tipAmounts = [10, 15, 20, 25, 0]
    @State private var tipAmount = 15
    
    var totalPrice: String {
        let total = Double(order.total) * (1 + Double(tipAmount) / 100)
        return total.formatted(.currency(code: "GBP"))
    }
    
    var body: some View {
        Form {
            Section {
                Picker("How you gonna pay?", selection: $paymentType) {
                    ForEach(paymentTypes, id: \.self) {
                        Text($0)
                    }
                }
                
                Toggle("Add loyalty card", isOn: $addLoyaltyDetails.animation())
                if addLoyaltyDetails {
                    TextField("Enter your loyalty number", text: $loyaltyNumber)
                }
            }
            
            Section("Add a tip") {
                Picker("How much percent?", selection: $tipAmount) {
                    ForEach(tipAmounts, id: \.self) {
                        Text("\($0)%")
                    }
                }
                .pickerStyle(.segmented)
            }
            
            Section("Total: \(totalPrice)") {
                Button("Confirm order") {
                    paymentAlertShowing.toggle()
                }
            }
        }
        .navigationTitle("Payment")
        .navigationBarTitleDisplayMode(.inline)
        .alert("Order Confirmed", isPresented: $paymentAlertShowing) {
            
        } message: {
            Text("Your total was \(totalPrice) - thank you!")
        }
    }
}

struct CheckoutView_Previews: PreviewProvider {
    static var previews: some View {
        NavigationStack {
            CheckoutView()
                .environmentObject(Order())
        }
    }
}
