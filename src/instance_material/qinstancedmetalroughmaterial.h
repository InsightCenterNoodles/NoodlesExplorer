// Original file from QT 3D extras. Modified to support basic instance
// rendering.

// Orig license below

/****************************************************************************
**
** Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/


#pragma once


#include <Qt3DExtras/qt3dextras_global.h>
#include <Qt3DRender/QAbstractTexture>
#include <Qt3DRender/QMaterial>
#include <QtGui/QColor>

#include <QShaderProgramBuilder>
#include <QTechnique>

class QInstancedMetalRoughMaterial : public Qt3DRender::QMaterial {
    Q_OBJECT

    Qt3DRender::QParameter*     m_baseColorParameter;
    Qt3DRender::QParameter*     m_metalnessParameter;
    Qt3DRender::QParameter*     m_roughnessParameter;
    Qt3DRender::QParameter*     m_baseColorMapParameter;
    Qt3DRender::QParameter*     m_metalnessMapParameter;
    Qt3DRender::QParameter*     m_roughnessMapParameter;
    Qt3DRender::QParameter*     m_ambientOcclusionMapParameter;
    Qt3DRender::QParameter*     m_normalMapParameter;
    Qt3DRender::QParameter*     m_textureScaleParameter;
    Qt3DRender::QEffect*        m_metalRoughEffect;
    Qt3DRender::QTechnique*     m_metalRoughGL3Technique;
    Qt3DRender::QRenderPass*    m_metalRoughGL3RenderPass;
    Qt3DRender::QShaderProgram* m_metalRoughGL3Shader;
    // Qt3DRender::QShaderProgramBuilder* m_metalRoughGL3ShaderBuilder;
    Qt3DRender::QFilterKey* m_filterKey;


    Q_PROPERTY(QVariant baseColor READ baseColor WRITE setBaseColor NOTIFY
                   baseColorChanged)
    Q_PROPERTY(QVariant metalness READ metalness WRITE setMetalness NOTIFY
                   metalnessChanged)
    Q_PROPERTY(QVariant roughness READ roughness WRITE setRoughness NOTIFY
                   roughnessChanged)
    Q_PROPERTY(
        QVariant ambientOcclusion READ ambientOcclusion WRITE
            setAmbientOcclusion NOTIFY ambientOcclusionChanged REVISION 10)
    Q_PROPERTY(QVariant normal READ normal WRITE setNormal NOTIFY normalChanged
                   REVISION 10)
    Q_PROPERTY(float textureScale READ textureScale WRITE setTextureScale NOTIFY
                   textureScaleChanged REVISION 10)

public:
    explicit QInstancedMetalRoughMaterial(Qt3DCore::QNode* parent = nullptr);
    ~QInstancedMetalRoughMaterial();

    QVariant baseColor() const;
    QVariant metalness() const;
    QVariant roughness() const;
    QVariant ambientOcclusion() const;
    QVariant normal() const;
    float    textureScale() const;

public slots:
    void setBaseColor(const QVariant& baseColor);
    void setMetalness(const QVariant& metalness);
    void setRoughness(const QVariant& roughness);
    void setAmbientOcclusion(const QVariant& ambientOcclusion);
    void setNormal(const QVariant& normal);
    void setTextureScale(float textureScale);

private slots:
    void handleTextureScaleChanged(const QVariant& var);

signals:
    void baseColorChanged(const QVariant& baseColor);
    void metalnessChanged(const QVariant& metalness);
    void roughnessChanged(const QVariant& roughness);
    void ambientOcclusionChanged(const QVariant& ambientOcclusion);
    void normalChanged(const QVariant& normal);
    void textureScaleChanged(float textureScale);
};
