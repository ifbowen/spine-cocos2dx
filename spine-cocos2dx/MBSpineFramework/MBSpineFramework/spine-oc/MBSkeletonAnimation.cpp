//
//  MBSkeletonAnimation.cpp
//  MBSpineFramework
//
//  Created by Bowen on 2019/10/24.
//  Copyright © 2019 inke. All rights reserved.
//

#include "MBSkeletonAnimation.h"
#include <spine/SkeletonTwoColorBatch.h>
#include <spine/AttachmentVertices.h>

USING_NS_CC;

MBSkeletonAnimation::MBSkeletonAnimation() : SkeletonAnimation() {
    
}

MBSkeletonAnimation::~MBSkeletonAnimation() {
    
}

MBSkeletonAnimation* MBSkeletonAnimation::createWithJsonFile (const std::string& skeletonJsonFile, const std::string& atlasFile, float scale) {
    MBSkeletonAnimation* node = new MBSkeletonAnimation();
    node->initWithJsonFile(skeletonJsonFile, atlasFile, scale);
    node->autorelease();
    return node;
}

void MBSkeletonAnimation::draw (Renderer* renderer, const Mat4& transform, uint32_t transformFlags) {
    SkeletonBatch* batch = SkeletonBatch::getInstance();
    SkeletonTwoColorBatch* twoColorBatch = SkeletonTwoColorBatch::getInstance();
    bool isTwoColorTint = this->isTwoColorTint();

    // Early exit if the skeleton is invisible
    if (getDisplayedOpacity() == 0 || _skeleton->getColor().a == 0){
        return;
    }
    
    if (_effect) _effect->begin(*_skeleton);
    
    Color4F nodeColor;
    nodeColor.r = getDisplayedColor().r / (float)255;
    nodeColor.g = getDisplayedColor().g / (float)255;
    nodeColor.b = getDisplayedColor().b / (float)255;
    nodeColor.a = getDisplayedOpacity() / (float)255;
    
    Color4F color;
    Color4F darkColor;
    float darkPremultipliedAlpha = _premultipliedAlpha ? 255 : 0;
    AttachmentVertices* attachmentVertices = nullptr;
    TwoColorTrianglesCommand* lastTwoColorTrianglesCommand = nullptr;
    bool inRange = _startSlotIndex != -1 || _endSlotIndex != -1 ? false : true;
    for (int i = 0, n = _skeleton->getSlots().size(); i < n; ++i) {
        Slot* slot = _skeleton->getDrawOrder()[i];
                
        if (_startSlotIndex >= 0 && _startSlotIndex == slot->getData().getIndex()) {
            inRange = true;
        }
        
        if (!inRange) {
            _clipper->clipEnd(*slot);
            continue;
        }
        
        if (_endSlotIndex >= 0 && _endSlotIndex == slot->getData().getIndex()) {
            inRange = false;
        }
        
        if (!slot->getAttachment()) {
            _clipper->clipEnd(*slot);
            continue;
        }
        
        // Early exit if slot is invisible
        if (slot->getColor().a == 0) {
            _clipper->clipEnd(*slot);
            continue;
        }
        
        replaceSkinImage(slot);
        
        cocos2d::TrianglesCommand::Triangles triangles;
        TwoColorTriangles trianglesTwoColor;
        
        if (slot->getAttachment()->getRTTI().isExactly(RegionAttachment::rtti)) {
            RegionAttachment* attachment = (RegionAttachment*)slot->getAttachment();
            attachmentVertices = (AttachmentVertices*)attachment->getRendererObject();
            
            // Early exit if attachment is invisible
            if (attachment->getColor().a == 0) {
                _clipper->clipEnd(*slot);
                continue;
            }
            
            if (!isTwoColorTint) {
                triangles.indices = attachmentVertices->_triangles->indices;
                triangles.indexCount = attachmentVertices->_triangles->indexCount;
                triangles.verts = batch->allocateVertices(attachmentVertices->_triangles->vertCount);
                triangles.vertCount = attachmentVertices->_triangles->vertCount;
                memcpy(triangles.verts, attachmentVertices->_triangles->verts, sizeof(cocos2d::V3F_C4B_T2F) * attachmentVertices->_triangles->vertCount);
                attachment->computeWorldVertices(slot->getBone(), (float*)triangles.verts, 0, 6);
            } else {
                trianglesTwoColor.indices = attachmentVertices->_triangles->indices;
                trianglesTwoColor.indexCount = attachmentVertices->_triangles->indexCount;
                trianglesTwoColor.verts = twoColorBatch->allocateVertices(attachmentVertices->_triangles->vertCount);
                trianglesTwoColor.vertCount = attachmentVertices->_triangles->vertCount;
                for (int i = 0; i < trianglesTwoColor.vertCount; i++) {
                    trianglesTwoColor.verts[i].texCoords = attachmentVertices->_triangles->verts[i].texCoords;
                }
                attachment->computeWorldVertices(slot->getBone(), (float*)trianglesTwoColor.verts, 0, 7);
            }
            
            color.r = attachment->getColor().r;
            color.g = attachment->getColor().g;
            color.b = attachment->getColor().b;
            color.a = attachment->getColor().a;
        }
        else if (slot->getAttachment()->getRTTI().isExactly(MeshAttachment::rtti)) {
            MeshAttachment* attachment = (MeshAttachment*)slot->getAttachment();
            attachmentVertices = (AttachmentVertices*)attachment->getRendererObject();
            
            // Early exit if attachment is invisible
            if (attachment->getColor().a == 0) {
                _clipper->clipEnd(*slot);
                continue;
            }
            
            if (!isTwoColorTint) {
                triangles.indices = attachmentVertices->_triangles->indices;
                triangles.indexCount = attachmentVertices->_triangles->indexCount;
                triangles.verts = batch->allocateVertices(attachmentVertices->_triangles->vertCount);
                triangles.vertCount = attachmentVertices->_triangles->vertCount;
                memcpy(triangles.verts, attachmentVertices->_triangles->verts, sizeof(cocos2d::V3F_C4B_T2F) * attachmentVertices->_triangles->vertCount);
                attachment->computeWorldVertices(*slot, 0, attachment->getWorldVerticesLength(), (float*)triangles.verts, 0, 6);
            } else {
                trianglesTwoColor.indices = attachmentVertices->_triangles->indices;
                trianglesTwoColor.indexCount = attachmentVertices->_triangles->indexCount;
                trianglesTwoColor.verts = twoColorBatch->allocateVertices(attachmentVertices->_triangles->vertCount);
                trianglesTwoColor.vertCount = attachmentVertices->_triangles->vertCount;
                for (int i = 0; i < trianglesTwoColor.vertCount; i++) {
                    trianglesTwoColor.verts[i].texCoords = attachmentVertices->_triangles->verts[i].texCoords;
                }
                attachment->computeWorldVertices(*slot, 0,  attachment->getWorldVerticesLength(), (float*)trianglesTwoColor.verts, 0, 7);
            }
            
            color.r = attachment->getColor().r;
            color.g = attachment->getColor().g;
            color.b = attachment->getColor().b;
            color.a = attachment->getColor().a;
        }
        else if (slot->getAttachment()->getRTTI().isExactly(ClippingAttachment::rtti)) {
            ClippingAttachment* clip = (ClippingAttachment*)slot->getAttachment();
            _clipper->clipStart(*slot, clip);
            continue;
        } else {
            _clipper->clipEnd(*slot);
            continue;
        }
        
        float alpha = nodeColor.a * _skeleton->getColor().a * slot->getColor().a * color.a * 255;
        // skip rendering if the color of this attachment is 0
        if (alpha == 0){
            _clipper->clipEnd(*slot);
            continue;
        }
        float multiplier = _premultipliedAlpha ? alpha : 255;
        float red = nodeColor.r * _skeleton->getColor().r * color.r * multiplier;
        float green = nodeColor.g * _skeleton->getColor().g * color.g * multiplier;
        float blue = nodeColor.b * _skeleton->getColor().b * color.b * multiplier;
        
        color.r = red * slot->getColor().r;
        color.g = green * slot->getColor().g;
        color.b = blue * slot->getColor().b;
        color.a = alpha;
        
        if (slot->hasDarkColor()) {
            darkColor.r = red * slot->getDarkColor().r;
            darkColor.g = green * slot->getDarkColor().g;
            darkColor.b = blue * slot->getDarkColor().b;
        } else {
            darkColor.r = 0;
            darkColor.g = 0;
            darkColor.b = 0;
        }
        darkColor.a = darkPremultipliedAlpha;
        
        BlendFunc blendFunc;
        switch (slot->getData().getBlendMode()) {
            case BlendMode_Additive:
                blendFunc.src = _premultipliedAlpha ? GL_ONE : GL_SRC_ALPHA;
                blendFunc.dst = GL_ONE;
                break;
            case BlendMode_Multiply:
                blendFunc.src = GL_DST_COLOR;
                blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
                break;
            case BlendMode_Screen:
                blendFunc.src = GL_ONE;
                blendFunc.dst = GL_ONE_MINUS_SRC_COLOR;
                break;
            default:
                blendFunc.src = _premultipliedAlpha ? GL_ONE : GL_SRC_ALPHA;
                blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
        }
                        
        if (!isTwoColorTint) {
            if (_clipper->isClipping()) {
                _clipper->clipTriangles((float*)&triangles.verts[0].vertices, triangles.indices, triangles.indexCount, (float*)&triangles.verts[0].texCoords, sizeof(cocos2d::V3F_C4B_T2F) / 4);
                batch->deallocateVertices(triangles.vertCount);
                
                if (_clipper->getClippedTriangles().size() == 0){
                    _clipper->clipEnd(*slot);
                    continue;
                }
                
                triangles.vertCount = _clipper->getClippedVertices().size() >> 1;
                triangles.verts = batch->allocateVertices(triangles.vertCount);
                triangles.indexCount = _clipper->getClippedTriangles().size();
                triangles.indices = batch->allocateIndices(triangles.indexCount);
                memcpy(triangles.indices, _clipper->getClippedTriangles().buffer(), sizeof(unsigned short) * _clipper->getClippedTriangles().size());
                
                cocos2d::TrianglesCommand* batchedTriangles = batch->addCommand(renderer, _globalZOrder, attachmentVertices->_texture, _glProgramState, blendFunc, triangles, transform, transformFlags);
                
                float* verts = _clipper->getClippedVertices().buffer();
                float* uvs = _clipper->getClippedUVs().buffer();
                if (_effect) {
                    Color light;
                    Color dark;
                    light.r = color.r / 255.0f;
                    light.g = color.g / 255.0f;
                    light.b = color.b / 255.0f;
                    light.a = color.a / 255.0f;
                    dark.r = dark.g = dark.b = dark.a = 0;
                    for (int v = 0, vn = batchedTriangles->getTriangles().vertCount, vv = 0; v < vn; ++v, vv+=2) {
                        V3F_C4B_T2F* vertex = batchedTriangles->getTriangles().verts + v;
                        Color lightCopy = light;
                        Color darkCopy = dark;
                        vertex->vertices.x = verts[vv];
                        vertex->vertices.y = verts[vv + 1];
                        vertex->texCoords.u = uvs[vv];
                        vertex->texCoords.v = uvs[vv + 1];
                        _effect->transform(vertex->vertices.x, vertex->vertices.y, vertex->texCoords.u, vertex->texCoords.v, lightCopy, darkCopy);
                        vertex->colors.r = (GLubyte)(lightCopy.r * 255);
                        vertex->colors.g = (GLubyte)(lightCopy.g * 255);
                        vertex->colors.b = (GLubyte)(lightCopy.b * 255);
                        vertex->colors.a = (GLubyte)(lightCopy.a * 255);
                    }
                } else {
                    for (int v = 0, vn = batchedTriangles->getTriangles().vertCount, vv = 0; v < vn; ++v, vv+=2) {
                        V3F_C4B_T2F* vertex = batchedTriangles->getTriangles().verts + v;
                        vertex->vertices.x = verts[vv];
                        vertex->vertices.y = verts[vv + 1];
                        vertex->texCoords.u = uvs[vv];
                        vertex->texCoords.v = uvs[vv + 1];
                        vertex->colors.r = (GLubyte)color.r;
                        vertex->colors.g = (GLubyte)color.g;
                        vertex->colors.b = (GLubyte)color.b;
                        vertex->colors.a = (GLubyte)color.a;
                    }
                }
            } else {
                cocos2d::TrianglesCommand* batchedTriangles = batch->addCommand(renderer, _globalZOrder, attachmentVertices->_texture, _glProgramState, blendFunc, triangles, transform, transformFlags);
                
                if (_effect) {
                    Color light;
                    Color dark;
                    light.r = color.r / 255.0f;
                    light.g = color.g / 255.0f;
                    light.b = color.b / 255.0f;
                    light.a = color.a / 255.0f;
                    dark.r = dark.g = dark.b = dark.a = 0;
                    for (int v = 0, vn = batchedTriangles->getTriangles().vertCount; v < vn; ++v) {
                        V3F_C4B_T2F* vertex = batchedTriangles->getTriangles().verts + v;
                        Color lightCopy = light;
                        Color darkCopy = dark;
                        _effect->transform(vertex->vertices.x, vertex->vertices.y, vertex->texCoords.u, vertex->texCoords.v, lightCopy,  darkCopy);
                        vertex->colors.r = (GLubyte)(lightCopy.r * 255);
                        vertex->colors.g = (GLubyte)(lightCopy.g * 255);
                        vertex->colors.b = (GLubyte)(lightCopy.b * 255);
                        vertex->colors.a = (GLubyte)(lightCopy.a * 255);
                    }
                } else {
                    for (int v = 0, vn = batchedTriangles->getTriangles().vertCount; v < vn; ++v) {
                        V3F_C4B_T2F* vertex = batchedTriangles->getTriangles().verts + v;
                        vertex->colors.r = (GLubyte)color.r;
                        vertex->colors.g = (GLubyte)color.g;
                        vertex->colors.b = (GLubyte)color.b;
                        vertex->colors.a = (GLubyte)color.a;
                    }
                }
            }
        } else {
            if (_clipper->isClipping()) {
                _clipper->clipTriangles((float*)&trianglesTwoColor.verts[0].position, trianglesTwoColor.indices, trianglesTwoColor.indexCount, (float*)&trianglesTwoColor.verts[0].texCoords, sizeof(V3F_C4B_C4B_T2F) / 4);
                twoColorBatch->deallocateVertices(trianglesTwoColor.vertCount);
                
                if (_clipper->getClippedTriangles().size() == 0){
                    _clipper->clipEnd(*slot);
                    continue;
                }
                
                trianglesTwoColor.vertCount = _clipper->getClippedVertices().size() >> 1;
                trianglesTwoColor.verts = twoColorBatch->allocateVertices(trianglesTwoColor.vertCount);
                trianglesTwoColor.indexCount = _clipper->getClippedTriangles().size();
                trianglesTwoColor.indices = twoColorBatch->allocateIndices(trianglesTwoColor.indexCount);
                memcpy(trianglesTwoColor.indices, _clipper->getClippedTriangles().buffer(), sizeof(unsigned short) * _clipper->getClippedTriangles().size());
                
                TwoColorTrianglesCommand* batchedTriangles = lastTwoColorTrianglesCommand = twoColorBatch->addCommand(renderer, _globalZOrder, attachmentVertices->_texture->getName(), _glProgramState, blendFunc, trianglesTwoColor, transform, transformFlags);
                
                float* verts = _clipper->getClippedVertices().buffer();
                float* uvs = _clipper->getClippedUVs().buffer();
                
                if (_effect) {
                    Color light;
                    Color dark;
                    light.r = color.r / 255.0f;
                    light.g = color.g / 255.0f;
                    light.b = color.b / 255.0f;
                    light.a = color.a / 255.0f;
                    dark.r = darkColor.r / 255.0f;
                    dark.g = darkColor.g / 255.0f;
                    dark.b = darkColor.b / 255.0f;
                    dark.a = darkColor.a / 255.0f;
                    for (int v = 0, vn = batchedTriangles->getTriangles().vertCount, vv = 0; v < vn; ++v, vv += 2) {
                        V3F_C4B_C4B_T2F* vertex = batchedTriangles->getTriangles().verts + v;
                        Color lightCopy = light;
                        Color darkCopy = dark;
                        vertex->position.x = verts[vv];
                        vertex->position.y = verts[vv + 1];
                        vertex->texCoords.u = uvs[vv];
                        vertex->texCoords.v = uvs[vv + 1];
                        _effect->transform(vertex->position.x, vertex->position.y, vertex->texCoords.u, vertex->texCoords.v, lightCopy, darkCopy);
                        vertex->color.r = (GLubyte)(lightCopy.r * 255);
                        vertex->color.g = (GLubyte)(lightCopy.g * 255);
                        vertex->color.b = (GLubyte)(lightCopy.b * 255);
                        vertex->color.a = (GLubyte)(lightCopy.a * 255);
                        vertex->color2.r = (GLubyte)(darkCopy.r * 255);
                        vertex->color2.g = (GLubyte)(darkCopy.g * 255);
                        vertex->color2.b = (GLubyte)(darkCopy.b * 255);
                        vertex->color2.a = (GLubyte)darkColor.a;
                    }
                } else {
                    for (int v = 0, vn = batchedTriangles->getTriangles().vertCount, vv = 0; v < vn; ++v, vv += 2) {
                        V3F_C4B_C4B_T2F* vertex = batchedTriangles->getTriangles().verts + v;
                        vertex->position.x = verts[vv];
                        vertex->position.y = verts[vv + 1];
                        vertex->texCoords.u = uvs[vv];
                        vertex->texCoords.v = uvs[vv + 1];
                        vertex->color.r = (GLubyte)color.r;
                        vertex->color.g = (GLubyte)color.g;
                        vertex->color.b = (GLubyte)color.b;
                        vertex->color.a = (GLubyte)color.a;
                        vertex->color2.r = (GLubyte)darkColor.r;
                        vertex->color2.g = (GLubyte)darkColor.g;
                        vertex->color2.b = (GLubyte)darkColor.b;
                        vertex->color2.a = (GLubyte)darkColor.a;
                    }
                }
            } else {
                TwoColorTrianglesCommand* batchedTriangles = lastTwoColorTrianglesCommand = twoColorBatch->addCommand(renderer, _globalZOrder, attachmentVertices->_texture->getName(), _glProgramState, blendFunc, trianglesTwoColor, transform, transformFlags);
                
                if (_effect) {
                    Color light;
                    Color dark;
                    light.r = color.r / 255.0f;
                    light.g = color.g / 255.0f;
                    light.b = color.b / 255.0f;
                    light.a = color.a / 255.0f;
                    dark.r = darkColor.r / 255.0f;
                    dark.g = darkColor.g / 255.0f;
                    dark.b = darkColor.b / 255.0f;
                    dark.a = darkColor.a / 255.0f;
                    
                    for (int v = 0, vn = batchedTriangles->getTriangles().vertCount; v < vn; ++v) {
                        V3F_C4B_C4B_T2F* vertex = batchedTriangles->getTriangles().verts + v;
                        Color lightCopy = light;
                        Color darkCopy = dark;
                        _effect->transform(vertex->position.x, vertex->position.y, vertex->texCoords.u, vertex->texCoords.v, lightCopy, darkCopy);
                        vertex->color.r = (GLubyte)(lightCopy.r * 255);
                        vertex->color.g = (GLubyte)(lightCopy.g * 255);
                        vertex->color.b = (GLubyte)(lightCopy.b * 255);
                        vertex->color.a = (GLubyte)(lightCopy.a * 255);
                        vertex->color2.r = (GLubyte)(darkCopy.r * 255);
                        vertex->color2.g = (GLubyte)(darkCopy.g * 255);
                        vertex->color2.b = (GLubyte)(darkCopy.b * 255);
                        vertex->color2.a = (GLubyte)darkColor.a;
                    }
                } else {
                    for (int v = 0, vn = batchedTriangles->getTriangles().vertCount; v < vn; ++v) {
                        V3F_C4B_C4B_T2F* vertex = batchedTriangles->getTriangles().verts + v;
                        vertex->color.r = (GLubyte)color.r;
                        vertex->color.g = (GLubyte)color.g;
                        vertex->color.b = (GLubyte)color.b;
                        vertex->color.a = (GLubyte)color.a;
                        vertex->color2.r = (GLubyte)darkColor.r;
                        vertex->color2.g = (GLubyte)darkColor.g;
                        vertex->color2.b = (GLubyte)darkColor.b;
                        vertex->color2.a = (GLubyte)darkColor.a;
                    }
                }
            }
        }
        _clipper->clipEnd(*slot);
    }
    _clipper->clipEnd();
    
    if (lastTwoColorTrianglesCommand) {
        Node* parent = this->getParent();
        
        // We need to decide if we can postpone flushing the current
        // batch. We can postpone if the next sibling node is a
        // two color tinted skeleton with the same global-z.
        // The parent->getChildrenCount() > 100 check is a hack
        // as checking for a sibling is an O(n) operation, and if
        // all children of this nodes parent are skeletons, we
        // are in O(n2) territory.
        if (!parent || parent->getChildrenCount() > 100 || getChildrenCount() != 0) {
            lastTwoColorTrianglesCommand->setForceFlush(true);
        } else {
            cocos2d::Vector<Node*>& children = parent->getChildren();
            Node* sibling = nullptr;
            for (ssize_t i = 0; i < children.size(); i++) {
                if (children.at(i) == this) {
                    if (i < children.size() - 1) {
                        sibling = children.at(i+1);
                        break;
                    }
                }
            }
            if (!sibling) {
                lastTwoColorTrianglesCommand->setForceFlush(true);
            } else {
                SkeletonRenderer* siblingSkeleton = dynamic_cast<SkeletonRenderer*>(sibling);
                if (!siblingSkeleton || // flush is next sibling isn't a SkeletonRenderer
                    !siblingSkeleton->isTwoColorTint() || // flush if next sibling isn't two color tinted
                    !siblingSkeleton->isVisible() || // flush if next sibling is two color tinted but not visible
                    (siblingSkeleton->getGlobalZOrder() != this->getGlobalZOrder())) { // flush if next sibling is two color tinted but z-order differs
                    lastTwoColorTrianglesCommand->setForceFlush(true);
                }
            }
        }
    }
    
    if (_effect) _effect->end();
    
    if (_debugSlots || _debugBones || _debugMeshes) {
        drawDebug(renderer, transform, transformFlags);
    }
}

/*
 目前实现了二种绘制
 REGION:是一个矩形图片的渲染，所以直接用类中mUsv来设置uvs
 MESH:usv需要计算一下，大概就是计算一下 x = (x-minx)/(maxx-minx), y = (y-miny)/(maxy-miny)
 */
void MBSkeletonAnimation::replaceSkinImage(Slot *slot) {
    
    bool isFind = false;
    std::string slotName = slot->getData().getName().buffer();
    if (skinMap.at(slotName)) {
        isFind = true;
    }
    if (isFind) {
        
        if (slot->getAttachment()->getRTTI().isExactly(RegionAttachment::rtti)) {
            RegionAttachment* attachment = (RegionAttachment*)slot->getAttachment();
            AttachmentVertices* attachmentVertices = (AttachmentVertices*)attachment->getRendererObject();
            
            if (attachment->getColor().a == 0) {
                _clipper->clipEnd(*slot);
                return;
            }
            
            Texture2D *texture = skinMap.at(slotName);
            attachmentVertices->_texture = texture;
            TrianglesCommand::Triangles* triangles = attachmentVertices->_triangles;
            
            triangles->verts[0].texCoords.u = 1;
            triangles->verts[0].texCoords.v = 1;
            triangles->verts[1].texCoords.u = 0;
            triangles->verts[1].texCoords.v = 1;
            triangles->verts[2].texCoords.u = 0;
            triangles->verts[2].texCoords.v = 0;
            triangles->verts[3].texCoords.u = 1;
            triangles->verts[3].texCoords.v = 0;
            
        }
        
        if (slot->getAttachment()->getRTTI().isExactly(MeshAttachment::rtti)) {
            MeshAttachment* attachment = (MeshAttachment*)slot->getAttachment();
            AttachmentVertices* attachmentVertices = (AttachmentVertices*)attachment->getRendererObject();
            
            if (attachment->getColor().a == 0) {
                _clipper->clipEnd(*slot);
                return;
            }
            
            Texture2D *texture = skinMap.at(slotName);
            attachmentVertices->_texture = texture;
                        
            float minX = 1.0, maxX = 0.0, minY = 1.0, maxY = 0.0;
            for (int i = 0; i < attachmentVertices->_triangles->vertCount; i++) {
                TrianglesCommand::Triangles* triangles = attachmentVertices->_triangles;
                if (triangles->verts[i].texCoords.u < minX) { minX = triangles->verts[i].texCoords.u; }
                if (triangles->verts[i].texCoords.u > maxX) { maxX = triangles->verts[i].texCoords.u; }
                if (triangles->verts[i].texCoords.v < minY) { minY = triangles->verts[i].texCoords.v; }
                if (triangles->verts[i].texCoords.v > maxY) { maxY = triangles->verts[i].texCoords.v; }
            }
            
            float lenx = maxX - minX;
            float leny = maxY - minY;
            for (int i = 0; i < attachmentVertices->_triangles->vertCount; i++) {
                TrianglesCommand::Triangles* triangles = attachmentVertices->_triangles;
                triangles->verts[i].texCoords.u -= minX;
                triangles->verts[i].texCoords.u /= lenx;
                triangles->verts[i].texCoords.v -= minY;
                triangles->verts[i].texCoords.v /= leny;
            }
        }
        
        if (slot->getAttachment()->getRTTI().isExactly(ClippingAttachment::rtti)) {
            printf("clipping渲染方法未实现\n");
        }
    }
}

