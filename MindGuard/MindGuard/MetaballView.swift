//
//  MetaballView.swift
//  MindGuard
//
//  Created by Richard Henry on 05/06/2023.
//

import SwiftUI

struct OffsetKey: PreferenceKey {
    static var defaultValue: CGRect = .zero
    static func reduce(value: inout CGRect, nextValue: () -> CGRect) {
        value = nextValue()
    }
}

struct AnchorKey: PreferenceKey {
    static var defaultValue: [String: Anchor<CGRect>] = [:]
    static func reduce(value: inout [String: Anchor<CGRect>], nextValue: () -> [String: Anchor<CGRect>]) {
        value.merge(nextValue()) { $1 }
    }
}

extension View {
    func offsetExtractor(coordinateSpace: String, completion: @escaping (CGRect) -> ()) -> some View {
        self
            .overlay {
                GeometryReader {
                    let rect = $0.frame(in: .named(coordinateSpace))
                    Color.clear
                        .preference(key: OffsetKey.self, value: rect)
                        .onPreferenceChange(OffsetKey.self, perform: completion)
                }
            }
    }
}

struct MetaballView: View {
    
    var body: some View {
        GeometryReader {
            
            MetaballScrollView(safeAreaInsets: $0.safeAreaInsets)
                .ignoresSafeArea()
        }
    }
}

struct MetaballScrollView: View {
    let safeAreaInsets: EdgeInsets
    
    @State private var scrollProgress: CGFloat = 0
    @Environment(\.colorScheme) private var colourScheme
    
    var body: some View {
        ScrollView(.vertical, showsIndicators: true) {
            VStack(spacing: 12) {
                let imageSize = 130 - (75 * scrollProgress)
                Image("LaoYang")
                    .resizable()
                    .aspectRatio(contentMode: .fill)
                    .frame(width: imageSize, height: imageSize)
                    .opacity(1 - scrollProgress)
                    .blur(radius: scrollProgress * 10, opaque: true)
                    .clipShape(Circle())
                    .anchorPreference(key: AnchorKey.self, value: .bounds, transform: { ["Header" : $0] })
                    .padding(.top, safeAreaInsets.top + 15)         // This has to be before the offset extractor so that the offset begins at 0
                    .offsetExtractor(coordinateSpace: "ScrollView") { scrollRect in scrollProgress = min(max(-scrollRect.minY / 25, 0), 1) }
                Text("It's all so tiresome.")
                    .font(.caption)
                    .foregroundColor(.gray)
                    .padding(.vertical, 15)
                SampleRows()
            }
            .frame(maxWidth: .infinity)
        }
        .backgroundPreferenceValue(AnchorKey.self, { pref in
            GeometryReader { proxy in
                if let anchor = pref["Header"] {
                    let frameRect = proxy[anchor]
                    Canvas { ctx, size in
                        ctx.addFilter(.alphaThreshold(min: 0.5)) // This is where the metaball magic happens.
                        ctx.addFilter(.blur(radius: 12))
                        ctx.drawLayer { ctx in
                            if let headerView = ctx.resolveSymbol(id: 0) {
                                ctx.draw(headerView, in: frameRect)
                            }
                            if let dynamicIsland = ctx.resolveSymbol(id: 1) {
                                ctx.draw(dynamicIsland, in: CGRect(x: (size.width - 120) * 0.5, y: 11, width: 120, height: 37))
                            }
                        }
                    } symbols: {
                        HeaderView(frameRect)
                            .tag(0)
                            .id(0)
                        
                        DynamicIslandCapsule()
                            .tag(1)
                            .id(1)
                    }
                }
            }
        })
        .overlay(alignment: .top) {
            Rectangle()
                .fill(colourScheme == .dark ? .black : .white)
                .frame(height: 15)
        }
        .coordinateSpace(name: "ScrollView")
    }
    
    @ViewBuilder func HeaderView(_ frameRect: CGRect) -> some View {
        Circle()
            .fill(.black)
            .frame(width: frameRect.width, height: frameRect.height)
    }
    
    @ViewBuilder func DynamicIslandCapsule(_ height: CGFloat = 37) -> some View {
        Capsule()
            .fill(.black)
            .frame(width: 120, height: height)
    }

    @ViewBuilder func SampleRows() -> some View {
        VStack {
            ForEach(1...23, id: \.self) { _ in
                VStack(alignment: .leading,spacing: 6) {
                    RoundedRectangle(cornerRadius: 5, style: .continuous)
                        .fill(.gray.opacity(0.15))
                        .frame(height: 25)
                    RoundedRectangle(cornerRadius: 5, style: .continuous)
                        .fill(.gray.opacity(0.15))
                        .frame(height: 15)
                        .padding(.trailing, 50)
                    RoundedRectangle(cornerRadius: 5, style: .continuous)
                        .fill(.gray.opacity(0.15))
                        .frame(height: 25)
                        .padding(.trailing, 150)
                }
            }
        }
        .padding(.horizontal, 15)
        .padding(.bottom, safeAreaInsets.bottom + 15)
    }
}

struct MetaballView_Previews: PreviewProvider {
    static var previews: some View {
        Group {
            MetaballView()
                .previewDevice(PreviewDevice(rawValue: "iPhone 14"))
                .previewDisplayName("Notch")
            MetaballView()
                .previewDevice(PreviewDevice(rawValue: "iPhone 14 Pro"))
                .previewDisplayName("Island")
        }
    }
}
