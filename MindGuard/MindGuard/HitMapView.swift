//
//  HitMapView.swift
//  MindGuard
//
//  Created by Richard Henry on 29/03/2023.
//

import SwiftUI
import MapKit

struct HitMapView: View {
    @StateObject var locationManager = LocationManager.shared
    @State private var trackingMode: MapUserTrackingMode = .follow
    @State private var sheetPresented = false

    var body: some View {
        ZStack {
            VStack {
                Map(coordinateRegion: $locationManager.region, interactionModes: .all, showsUserLocation: true, userTrackingMode: $trackingMode)
                    .sheet(isPresented: $sheetPresented) {
                        Text("Detail")
                            .presentationDetents([.medium, .large])
                    }
            }
            VStack(alignment: .leading) {
                HStack {
                    Button("Scan") {
                        sheetPresented.toggle()
                    }
                    .padding(.top, 60)
                    .padding(.leading, 7)
                    Spacer()
                }
                Spacer()
            }
        }
        .ignoresSafeArea(.all, edges: [.top, .trailing, .leading])
    }
}

struct HitMapView_Previews: PreviewProvider {
    static var previews: some View {
        TabView {
            HitMapView()
                .tabItem { Label("HitMap", systemImage: "list.dash") }
        }
    }
}

// MARK: -

class LocationManager: NSObject, ObservableObject, CLLocationManagerDelegate {
    static var shared = LocationManager()
    @Published var region = MKCoordinateRegion(center: CLLocationCoordinate2D(latitude: 51.507222, longitude: -0.1275), span: MKCoordinateSpan(latitudeDelta: 0.05, longitudeDelta: 0.05))
    private let manager = CLLocationManager()
    
    override init() {
        super.init()
        manager.delegate = self
        manager.desiredAccuracy = kCLLocationAccuracyBest
        manager.requestWhenInUseAuthorization()
        manager.startUpdatingLocation()
    }
    
    func locationManager(_ manager: CLLocationManager, didUpdateLocations locations: [CLLocation]) {
        locations.last.map {
            region.center = CLLocationCoordinate2D(latitude: $0.coordinate.latitude, longitude: $0.coordinate.longitude)
        }
        manager.stopUpdatingLocation()
    }
}
