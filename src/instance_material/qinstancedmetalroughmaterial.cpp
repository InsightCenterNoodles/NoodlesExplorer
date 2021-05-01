#include "qinstancedmetalroughmaterial.h"

#include <QUrl>
#include <QVector3D>
#include <QVector4D>
#include <Qt3DRender/qeffect.h>
#include <Qt3DRender/qfilterkey.h>
#include <Qt3DRender/qgraphicsapifilter.h>
#include <Qt3DRender/qmaterial.h>
#include <Qt3DRender/qparameter.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qshaderprogram.h>
#include <Qt3DRender/qshaderprogrambuilder.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qtexture.h>

using namespace Qt3DRender;

QInstancedMetalRoughMaterial::QInstancedMetalRoughMaterial(
    Qt3DCore::QNode* parent)
    : Qt3DRender::QMaterial(parent),

      m_baseColorParameter(
          new QParameter(QStringLiteral("baseColor"), QColor("grey"))),
      m_metalnessParameter(new QParameter(QStringLiteral("metalness"), 0.0f)),
      m_roughnessParameter(new QParameter(QStringLiteral("roughness"), 0.0f)),
      m_baseColorMapParameter(
          new QParameter(QStringLiteral("baseColorMap"), QVariant())),
      m_metalnessMapParameter(
          new QParameter(QStringLiteral("metalnessMap"), QVariant())),
      m_roughnessMapParameter(
          new QParameter(QStringLiteral("roughnessMap"), QVariant())),
      m_ambientOcclusionMapParameter(
          new QParameter(QStringLiteral("ambientOcclusionMap"), QVariant())),
      m_normalMapParameter(
          new QParameter(QStringLiteral("normalMap"), QVariant())),
      m_textureScaleParameter(
          new QParameter(QStringLiteral("texCoordScale"), 1.0f)),
      m_metalRoughEffect(new QEffect()),
      m_metalRoughGL3Technique(new QTechnique()),
      m_metalRoughGL3RenderPass(new QRenderPass()),
      m_metalRoughGL3Shader(new QShaderProgram()),
      m_metalRoughGL3ShaderBuilder(new QShaderProgramBuilder()),
      m_metalRoughES3Technique(new QTechnique()),
      m_metalRoughES3RenderPass(new QRenderPass()),
      m_metalRoughES3Shader(new QShaderProgram()),
      m_metalRoughES3ShaderBuilder(new QShaderProgramBuilder()),
      m_metalRoughRHITechnique(new QTechnique()),
      m_metalRoughRHIRenderPass(new QRenderPass()),
      m_metalRoughRHIShader(new QShaderProgram()),
      m_metalRoughRHIShaderBuilder(new QShaderProgramBuilder()),
      m_filterKey(new QFilterKey) {


    QObject::connect(m_baseColorParameter,
                     &Qt3DRender::QParameter::valueChanged,
                     this,
                     &QInstancedMetalRoughMaterial::baseColorChanged);
    QObject::connect(m_metalnessParameter,
                     &Qt3DRender::QParameter::valueChanged,
                     this,
                     &QInstancedMetalRoughMaterial::metalnessChanged);
    QObject::connect(m_roughnessParameter,
                     &Qt3DRender::QParameter::valueChanged,
                     this,
                     &QInstancedMetalRoughMaterial::roughnessChanged);
    QObject::connect(m_ambientOcclusionMapParameter,
                     &Qt3DRender::QParameter::valueChanged,
                     this,
                     &QInstancedMetalRoughMaterial::roughnessChanged);
    QObject::connect(m_normalMapParameter,
                     &Qt3DRender::QParameter::valueChanged,
                     this,
                     &QInstancedMetalRoughMaterial::normalChanged);

    connect(m_textureScaleParameter,
            &Qt3DRender::QParameter::valueChanged,
            this,
            &QInstancedMetalRoughMaterial::handleTextureScaleChanged);

    m_metalRoughGL3Shader->setVertexShaderCode(QShaderProgram::loadSource(QUrl(
        QStringLiteral("qrc:/instance_material/shaders/gl3/default.vert"))));
    m_metalRoughGL3ShaderBuilder->setParent(this);
    m_metalRoughGL3ShaderBuilder->setShaderProgram(m_metalRoughGL3Shader);
    m_metalRoughGL3ShaderBuilder->setFragmentShaderGraph(QUrl(QStringLiteral(
        "qrc:/instance_material/shaders/graphs/metalrough.frag.json")));
    m_metalRoughGL3ShaderBuilder->setEnabledLayers(
        { QStringLiteral("baseColor"),
          QStringLiteral("metalness"),
          QStringLiteral("roughness"),
          QStringLiteral("ambientOcclusion"),
          QStringLiteral("normal") });

    m_metalRoughES3Shader->setVertexShaderCode(QShaderProgram::loadSource(QUrl(
        QStringLiteral("qrc:/instance_material/shaders/es3/default.vert"))));
    m_metalRoughES3ShaderBuilder->setParent(this);
    m_metalRoughES3ShaderBuilder->setShaderProgram(m_metalRoughES3Shader);
    m_metalRoughES3ShaderBuilder->setFragmentShaderGraph(QUrl(QStringLiteral(
        "qrc:/instance_material/shaders/graphs/metalrough.frag.json")));
    m_metalRoughES3ShaderBuilder->setEnabledLayers(
        { QStringLiteral("baseColor"),
          QStringLiteral("metalness"),
          QStringLiteral("roughness"),
          QStringLiteral("ambientOcclusion"),
          QStringLiteral("normal") });

    m_metalRoughRHIShader->setVertexShaderCode(QShaderProgram::loadSource(QUrl(
        QStringLiteral("qrc:/instance_material/shaders/rhi/default.vert"))));
    m_metalRoughRHIShaderBuilder->setParent(this);
    m_metalRoughRHIShaderBuilder->setShaderProgram(m_metalRoughRHIShader);
    m_metalRoughRHIShaderBuilder->setFragmentShaderGraph(QUrl(QStringLiteral(
        "qrc:/instance_material/shaders/graphs/metalrough.frag.json")));
    m_metalRoughRHIShaderBuilder->setEnabledLayers(
        { QStringLiteral("baseColor"),
          QStringLiteral("metalness"),
          QStringLiteral("roughness"),
          QStringLiteral("ambientOcclusion"),
          QStringLiteral("normal") });

    m_metalRoughGL3Technique->graphicsApiFilter()->setApi(
        QGraphicsApiFilter::OpenGL);
    m_metalRoughGL3Technique->graphicsApiFilter()->setMajorVersion(3);
    m_metalRoughGL3Technique->graphicsApiFilter()->setMinorVersion(1);
    m_metalRoughGL3Technique->graphicsApiFilter()->setProfile(
        QGraphicsApiFilter::CoreProfile);

    m_metalRoughES3Technique->graphicsApiFilter()->setApi(
        QGraphicsApiFilter::OpenGLES);
    m_metalRoughES3Technique->graphicsApiFilter()->setMajorVersion(3);
    m_metalRoughES3Technique->graphicsApiFilter()->setMinorVersion(0);

    m_metalRoughRHITechnique->graphicsApiFilter()->setApi(
        QGraphicsApiFilter::RHI);
    m_metalRoughRHITechnique->graphicsApiFilter()->setMajorVersion(1);
    m_metalRoughRHITechnique->graphicsApiFilter()->setMinorVersion(0);

    m_filterKey->setParent(this);
    m_filterKey->setName(QStringLiteral("renderingStyle"));
    m_filterKey->setValue(QStringLiteral("forward"));

    m_metalRoughGL3Technique->addFilterKey(m_filterKey);
    m_metalRoughGL3RenderPass->setShaderProgram(m_metalRoughGL3Shader);
    m_metalRoughGL3Technique->addRenderPass(m_metalRoughGL3RenderPass);
    m_metalRoughEffect->addTechnique(m_metalRoughGL3Technique);

    m_metalRoughES3Technique->addFilterKey(m_filterKey);
    m_metalRoughES3RenderPass->setShaderProgram(m_metalRoughES3Shader);
    m_metalRoughES3Technique->addRenderPass(m_metalRoughES3RenderPass);
    m_metalRoughEffect->addTechnique(m_metalRoughES3Technique);

    m_metalRoughRHITechnique->addFilterKey(m_filterKey);
    m_metalRoughRHIRenderPass->setShaderProgram(m_metalRoughRHIShader);
    m_metalRoughRHITechnique->addRenderPass(m_metalRoughRHIRenderPass);
    m_metalRoughEffect->addTechnique(m_metalRoughRHITechnique);

    // Given parameters a parent
    m_baseColorMapParameter->setParent(m_metalRoughEffect);
    m_metalnessMapParameter->setParent(m_metalRoughEffect);
    m_roughnessMapParameter->setParent(m_metalRoughEffect);

    m_metalRoughEffect->addParameter(m_baseColorParameter);
    m_metalRoughEffect->addParameter(m_metalnessParameter);
    m_metalRoughEffect->addParameter(m_roughnessParameter);
    m_metalRoughEffect->addParameter(m_textureScaleParameter);

    this->setEffect(m_metalRoughEffect);
}

void QInstancedMetalRoughMaterial::handleTextureScaleChanged(
    const QVariant& var) {
    emit this->textureScaleChanged(var.toFloat());
}

QInstancedMetalRoughMaterial::~QInstancedMetalRoughMaterial() { }

QVariant QInstancedMetalRoughMaterial::baseColor() const {
    return m_baseColorParameter->value();
}

QVariant QInstancedMetalRoughMaterial::metalness() const {
    return m_metalnessParameter->value();
}

QVariant QInstancedMetalRoughMaterial::roughness() const {
    return m_roughnessParameter->value();
}

QVariant QInstancedMetalRoughMaterial::ambientOcclusion() const {
    return m_ambientOcclusionMapParameter->value();
}

QVariant QInstancedMetalRoughMaterial::normal() const {
    return m_normalMapParameter->value();
}

float QInstancedMetalRoughMaterial::textureScale() const {
    return m_textureScaleParameter->value().toFloat();
}

void QInstancedMetalRoughMaterial::setBaseColor(const QVariant& baseColor) {
    m_baseColorParameter->setValue(baseColor);
    m_baseColorMapParameter->setValue(baseColor);

    auto layers = m_metalRoughGL3ShaderBuilder->enabledLayers();
    if (baseColor.value<QAbstractTexture*>()) {
        layers.removeAll(QStringLiteral("baseColor"));
        layers.append(QStringLiteral("baseColorMap"));
        m_metalRoughEffect->addParameter(m_baseColorMapParameter);
        if (m_metalRoughEffect->parameters().contains(m_baseColorParameter))
            m_metalRoughEffect->removeParameter(m_baseColorParameter);
    } else {
        layers.removeAll(QStringLiteral("baseColorMap"));
        layers.append(QStringLiteral("baseColor"));
        if (m_metalRoughEffect->parameters().contains(m_baseColorMapParameter))
            m_metalRoughEffect->removeParameter(m_baseColorMapParameter);
        m_metalRoughEffect->addParameter(m_baseColorParameter);
    }
    m_metalRoughGL3ShaderBuilder->setEnabledLayers(layers);
    m_metalRoughES3ShaderBuilder->setEnabledLayers(layers);
    m_metalRoughRHIShaderBuilder->setEnabledLayers(layers);
}

void QInstancedMetalRoughMaterial::setMetalness(const QVariant& metalness) {
    m_metalnessParameter->setValue(metalness);
    m_metalnessMapParameter->setValue(metalness);

    auto layers = m_metalRoughGL3ShaderBuilder->enabledLayers();
    if (metalness.value<QAbstractTexture*>()) {
        layers.removeAll(QStringLiteral("metalness"));
        layers.append(QStringLiteral("metalnessMap"));
        m_metalRoughEffect->addParameter(m_metalnessMapParameter);
        if (m_metalRoughEffect->parameters().contains(m_metalnessParameter))
            m_metalRoughEffect->removeParameter(m_metalnessParameter);
    } else {
        layers.removeAll(QStringLiteral("metalnessMap"));
        layers.append(QStringLiteral("metalness"));
        if (m_metalRoughEffect->parameters().contains(m_metalnessMapParameter))
            m_metalRoughEffect->removeParameter(m_metalnessMapParameter);
        m_metalRoughEffect->addParameter(m_metalnessParameter);
    }
    m_metalRoughGL3ShaderBuilder->setEnabledLayers(layers);
    m_metalRoughES3ShaderBuilder->setEnabledLayers(layers);
    m_metalRoughRHIShaderBuilder->setEnabledLayers(layers);
}

void QInstancedMetalRoughMaterial::setRoughness(const QVariant& roughness) {
    m_roughnessParameter->setValue(roughness);
    m_roughnessMapParameter->setValue(roughness);

    auto layers = m_metalRoughGL3ShaderBuilder->enabledLayers();
    if (roughness.value<QAbstractTexture*>()) {
        layers.removeAll(QStringLiteral("roughness"));
        layers.append(QStringLiteral("roughnessMap"));
        m_metalRoughEffect->addParameter(m_roughnessMapParameter);
        if (m_metalRoughEffect->parameters().contains(m_roughnessParameter))
            m_metalRoughEffect->removeParameter(m_roughnessParameter);
    } else {
        layers.removeAll(QStringLiteral("roughnessMap"));
        layers.append(QStringLiteral("roughness"));
        if (m_metalRoughEffect->parameters().contains(m_roughnessMapParameter))
            m_metalRoughEffect->removeParameter(m_roughnessMapParameter);
        m_metalRoughEffect->addParameter(m_roughnessParameter);
    }
    m_metalRoughGL3ShaderBuilder->setEnabledLayers(layers);
    m_metalRoughES3ShaderBuilder->setEnabledLayers(layers);
    m_metalRoughRHIShaderBuilder->setEnabledLayers(layers);
}

void QInstancedMetalRoughMaterial::setAmbientOcclusion(
    const QVariant& ambientOcclusion) {
    m_ambientOcclusionMapParameter->setValue(ambientOcclusion);

    auto layers = m_metalRoughGL3ShaderBuilder->enabledLayers();
    if (ambientOcclusion.value<QAbstractTexture*>()) {
        layers.removeAll(QStringLiteral("ambientOcclusion"));
        layers.append(QStringLiteral("ambientOcclusionMap"));
        m_metalRoughEffect->addParameter(m_ambientOcclusionMapParameter);
    } else {
        layers.removeAll(QStringLiteral("ambientOcclusionMap"));
        layers.append(QStringLiteral("ambientOcclusion"));
        if (m_metalRoughEffect->parameters().contains(
                m_ambientOcclusionMapParameter))
            m_metalRoughEffect->removeParameter(m_ambientOcclusionMapParameter);
    }
    m_metalRoughGL3ShaderBuilder->setEnabledLayers(layers);
    m_metalRoughES3ShaderBuilder->setEnabledLayers(layers);
    m_metalRoughRHIShaderBuilder->setEnabledLayers(layers);
}

void QInstancedMetalRoughMaterial::setNormal(const QVariant& normal) {
    m_normalMapParameter->setValue(normal);

    auto layers = m_metalRoughGL3ShaderBuilder->enabledLayers();
    if (normal.value<QAbstractTexture*>()) {
        layers.removeAll(QStringLiteral("normal"));
        layers.append(QStringLiteral("normalMap"));
        m_metalRoughEffect->addParameter(m_normalMapParameter);
    } else {
        layers.removeAll(QStringLiteral("normalMap"));
        layers.append(QStringLiteral("normal"));
        if (m_metalRoughEffect->parameters().contains(m_normalMapParameter))
            m_metalRoughEffect->removeParameter(m_normalMapParameter);
    }
    m_metalRoughGL3ShaderBuilder->setEnabledLayers(layers);
    m_metalRoughES3ShaderBuilder->setEnabledLayers(layers);
    m_metalRoughRHIShaderBuilder->setEnabledLayers(layers);
}

void QInstancedMetalRoughMaterial::setTextureScale(float textureScale) {
    m_textureScaleParameter->setValue(textureScale);
}
