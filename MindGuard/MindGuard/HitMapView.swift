//
//  HitMapView.swift
//  MindGuard
//
//  Created by Richard Henry on 29/03/2023.
//

import SwiftUI
import MapKit

struct HitMapView: View {
    @State private var region = MKCoordinateRegion(center: CLLocationCoordinate2D(latitude: 51.507222, longitude: -0.1275), span: MKCoordinateSpan(latitudeDelta: 0.5, longitudeDelta: 0.5))

    var body: some View {
        VStack {
            Map(coordinateRegion: $region)
        }
        .ignoresSafeArea(.all, edges: [.top, .trailing, .leading])
    }
}

struct HitMapView_Previews: PreviewProvider {
    static var previews: some View {
        HitMapView()
    }
}
