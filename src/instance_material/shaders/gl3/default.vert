/****************************************************************************
**
** Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#version 150 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexTangent;
in vec2 vertexTexCoord;
in vec4 vertexColor;
in mat4 raw_instance;

out vec3 worldPosition;
out vec3 worldNormal;
out vec4 worldTangent;
out vec4 intColor;
out vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat3 modelNormalMatrix;
uniform mat4 modelViewProjection;

uniform float texCoordScale;

vec3 rotate_vertex_position(in vec3 v, in vec4 q) {
    return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v);
}

void main()
{
    vec3 inst_pos   = raw_instance[0].xyz;
    vec4 inst_color = raw_instance[1];
    vec4 inst_quat  = raw_instance[2];
    vec3 inst_scale = raw_instance[3].xyz;

    vec3 pos = rotate_vertex_position(vertexPosition * inst_scale , inst_quat)
                + inst_pos;

    vec3 nor  = rotate_vertex_position(vertexNormal      , inst_quat);
    vec3 tang = rotate_vertex_position(vertexTangent.xyz , inst_quat);

    // Pass through scaled texture coordinates
    texCoord = vertexTexCoord * texCoordScale;

    intColor = inst_color * vertexColor;

    // Transform position, normal, and tangent to world space
    worldPosition = vec3(modelMatrix * vec4(pos, 1.0));
    worldNormal = normalize(modelNormalMatrix * nor);
    worldTangent.xyz = normalize(vec3(modelMatrix * vec4(tang.xyz, 0.0)));
    worldTangent.w = vertexTangent.w;

    // Calculate vertex position in clip coordinates
    gl_Position = modelViewProjection * vec4(pos, 1.0);
}
