/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrClearStencilClipOp_DEFINED
#define GrClearStencilClipOp_DEFINED

#include "src/gpu/GrRenderTargetProxy.h"
#include "src/gpu/GrScissorState.h"
#include "src/gpu/ops/GrOp.h"

class GrOpFlushState;
class GrRecordingContext;

class GrClearStencilClipOp final : public GrOp {
public:
    DEFINE_OP_CLASS_ID

    static std::unique_ptr<GrOp> Make(GrRecordingContext* context,
                                      const GrScissorState& scissor,
                                      bool insideStencilMask,
                                      GrRenderTargetProxy* proxy);

    const char* name() const override { return "ClearStencilClip"; }

#ifdef SK_DEBUG
    SkString dumpInfo() const override {
        SkString string("Scissor [");
        if (fScissor.enabled()) {
            const SkIRect& r = fScissor.rect();
            string.appendf("L: %d, T: %d, R: %d, B: %d", r.fLeft, r.fTop, r.fRight, r.fBottom);
        } else {
            string.append("disabled");
        }
        string.appendf("], insideMask: %s\n", fInsideStencilMask ? "true" : "false");
        string.append(INHERITED::dumpInfo());
        return string;
    }
#endif

private:
    friend class GrOpMemoryPool; // for ctor

    GrClearStencilClipOp(const GrScissorState& scissor, bool insideStencilMask,
                         GrRenderTargetProxy* proxy)
            : INHERITED(ClassID())
            , fScissor(scissor)
            , fInsideStencilMask(insideStencilMask) {
        const SkRect& bounds =
                fScissor.enabled() ? SkRect::Make(fScissor.rect()) : proxy->getBoundsRect();
        this->setBounds(bounds, HasAABloat::kNo, IsHairline::kNo);
    }

    void onPrePrepare(GrRecordingContext*,
                      const GrSurfaceProxyView* writeView,
                      GrAppliedClip*,
                      const GrXferProcessor::DstProxyView&) override {}

    void onPrepare(GrOpFlushState*) override {}

    void onExecute(GrOpFlushState*, const SkRect& chainBounds) override;

    const GrScissorState fScissor;
    const bool           fInsideStencilMask;

    typedef GrOp INHERITED;
};

#endif
