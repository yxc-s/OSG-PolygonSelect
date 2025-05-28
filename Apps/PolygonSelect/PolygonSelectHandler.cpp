#include "PolygonSelectHandler.h"

struct PolygonSelectionVisualizer
{
    PolygonSelectionVisualizer(): 
		m_polygonPonits(new osg::Vec2Array),
		m_verticesPolygon(new osg::Vec3Array), 
        m_geometryPolygon(new osg::Geometry), m_verticesLine(new osg::Vec3Array), 
		m_drawArrayLine(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, 0)), 
		m_geometryLine(new osg::Geometry), m_geode(new osg::Geode)
    {
        m_geometryPolygon->setVertexArray(m_verticesPolygon);

        osg::Vec4Array *colors = new osg::Vec4Array;
        colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 0.3f));
        m_geometryPolygon->setColorArray(colors, osg::Array::BIND_OVERALL);
        m_geometryPolygon->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON, 0, 0));
        m_geometryPolygon->setDataVariance(osg::Object::DYNAMIC);
        m_geode->addDrawable(m_geometryPolygon);

        m_geometryLine->setVertexArray(m_verticesLine);
        m_geometryLine->setColorArray(colors, osg::Array::BIND_OVERALL);
        m_geometryLine->addPrimitiveSet(m_drawArrayLine);
        m_geometryLine->setDataVariance(osg::Object::DYNAMIC);
        m_geode->addDrawable(m_geometryLine);

		auto stateset = m_geode->getOrCreateStateSet();
		osg::Material *material = new osg::Material;
        material->setColorMode(osg::Material::OFF);
        material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0, 0.0f, 1.0f, 1.0f));
        material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0, 0.0f, 1.0f, 1.0f));
        material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0, 0.0f, 1.0f, 1.0f));
        material->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0, 1.0f, 1.0f, 1.0f));
        stateset->setAttributeAndModes(material, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);

        stateset->setMode(GL_LIGHTING, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF);
        stateset->setTextureMode(0, GL_TEXTURE_2D, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF);

        stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OVERRIDE | osg::StateAttribute::OFF);
        stateset->setRenderBinDetails(100, "RenderBin");

		osg::BlendFunc *blend = new osg::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        stateset->setAttributeAndModes(blend, osg::StateAttribute::ON);
	}
    


	void Render()
    {
        m_drawArrayLine->setCount(m_polygonPonits->size());
		m_verticesPolygon->clear();
        std::copy(m_verticesLine->begin(), m_verticesLine->end(), std::back_inserter(*m_verticesPolygon));

        auto numPrimitiveSets = m_geometryPolygon->getNumPrimitiveSets();
        for (int i = numPrimitiveSets - 1; i >= 0; --i)
        {
            m_geometryPolygon->removePrimitiveSet(i);
        }
        m_geometryPolygon->addPrimitiveSet(
            new osg::DrawArrays(osg::PrimitiveSet::POLYGON, 0, m_verticesPolygon->size()));

		if (m_verticesPolygon->size() > 3)
        {
            osg::ref_ptr<osgUtil::Tessellator> tessellator = new osgUtil::Tessellator();
            tessellator->setTessellationType(osgUtil::Tessellator::TESS_TYPE_POLYGONS);
            tessellator->setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD);
            tessellator->retessellatePolygons(*m_geometryPolygon);
        }

        m_verticesLine->dirty();
        m_verticesPolygon->dirty();
        m_geometryLine->dirtyGLObjects();
        m_geometryPolygon->dirtyGLObjects();

		 //osg::ref_ptr<osg::ShapeDrawable> testSphere =
   //      new osg::ShapeDrawable(new osg::Sphere(m_verticesLine->back(), 5)); // 创建一个中心在原点，半径 0.5的球体
   //      osg::ref_ptr<osg::StateSet> testStateSet = new osg::StateSet;
   //      testStateSet->setMode(GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
   //      osg::ref_ptr<osg::Material> testMaterial = new osg::Material;
   //      testMaterial->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 0.0, 0.0, 1.0));
   //      testStateSet->setAttributeAndModes(testMaterial, osg::StateAttribute::ON);
   //      testStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
   //      testStateSet->setRenderBinDetails(100, "RenderBin");
   //      osg::ref_ptr<osg::PolygonMode> polymode = new osg::PolygonMode;
   //      polymode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
   //      testStateSet->setAttributeAndModes(polymode, osg::StateAttribute::ON);
   //      osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth;
   //      lineWidth->setWidth(2.0);
   //      testStateSet->setAttributeAndModes(lineWidth, osg::StateAttribute::ON);
   //      testSphere->setStateSet(testStateSet);
   //      m_geode->addDrawable(testSphere.get());
	}


	void Clear()
    {
        m_verticesLine->clear();
        m_verticesPolygon->clear();
        m_polygonPonits->clear();
	}
	osg::ref_ptr<osg::Vec2Array> m_polygonPonits;
	osg::ref_ptr<osg::Vec3Array> m_verticesPolygon;
    osg::ref_ptr<osg::Geometry> m_geometryPolygon;
    osg::ref_ptr<osg::Vec3Array> m_verticesLine;
    osg::ref_ptr<osg::DrawArrays> m_drawArrayLine;
    osg::ref_ptr<osg::Geometry> m_geometryLine;
    osg::ref_ptr<osg::Geode> m_geode;
};
struct PolygonSelection
{
    int m_leftMouseButton = osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON;
    bool m_gotFirstLocation = false;
    bool m_lastPointTemporary = false;
    bool m_finished = false;
    float m_mouseDownX, m_mouseDownY;
    //osg::ref_ptr<osg::Camera> m_hudCamera;

    std::function<void(osg::Vec3)> m_pushVerticeCallback;
    std::function<void(osg::Vec3)> m_updateVerticeCallback;
    std::function<void()> m_clearCallback;
    std::function<void()> m_revokeCallback;
    //std::function<void()> m_doSelectCallback;
};


PolygonSelectHandler::PolygonSelectHandler(osg::ref_ptr root):
    m_root(root),
    m_polygonSelection(new PolygonSelection),
    m_polygonSelectionVisualizer(new PolygonSelectionVisualizer),
    bool m_isSelect(false)
 {

    m_polygonSelection->m_pushVerticeCallback = [&](const osg::Vec3& p) {
        m_polygonSelectionVisualizer->m_verticesLine->push_back(p);
        m_polygonSelectionVisualizer->m_polygonPonits->push_back(osg::Vec2(p[0], p[1]));
        m_polygonSelectionVisualizer->Render();
    }
    m_polygonSelection->m_updateVerticeCallback = [&](const osg::Vec3& p) {
        m_polygonSelectionVisualizer->m_verticesLine->back() = p;
        m_polygonSelectionVisualizer->m_polygonPonits->back() = osg::Vec2(p[0], p[1]);
        m_polygonSelectionVisualizer->Render();
    }    
    m_polygonSelection->m_clearCallback = [&]() {
        m_polygonSelectionVisualizer->Clear();
        m_polygonSelectionVisualizer->Render();
    }    
    m_polygonSelection->m_revokeCallback = [&]() {
        size_t count = m_polygonSelectionVisualizer->m_polygonPonits->size();
        if (count > 2)
        {
            m_polygonSelectionVisualizer->m_polygonPonits->pop_back();
            m_polygonSelectionVisualizer->m_verticesLine->at(count - 2) =
                m_polygonSelectionVisualizer->m_verticesLine->at(count - 1);
            m_polygonSelectionVisualizer->m_verticesLine->pop_back();
            m_polygonSelectionVisualizer->Render();
        }
    }    
}

PolygonSelectHandler::~PolygonSelectHandler() {}

PolygonSelectHandler::handle(const osgGA::EventAdapter& ea, osgGA::GUIActionAdapter) {
    switch (ea.getEventType()) {
        case osgGA::GUIEventAdapter::KEYDOWN:
            if (ea.getKey() == 's' || ea.getKey() == 'S') {
                m_isSelect = !m_isSelect;
                return true;
            }
            break;
        default:
            break;
    }

    if (m_isSelect) {
        osgViewer::Viewer *viewer = dynamic_cast<osgViewer::Viewer *>(&aa);
        auto event = ea.getEventType();
        osg::Vec3 worldPoint(ea.getX(), ea.getY(), 0);
        if (ea.getEventType() == osgGA::GUIEventAdapter::PUSH &&
            ea.getButton() == m_polygonSelection->m_leftMouseButton)
        {
            m_polygonSelection->m_mouseDownX = worldPoint[0];
            m_polygonSelection->m_mouseDownY = worldPoint[1];
        }
        else if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE &&
                    ea.getButton() == m_polygonSelection->m_leftMouseButton)
        {
            float eps = 1.0f;
            if (osg::equivalent((float)worldPoint[0], m_polygonSelection->m_mouseDownX, eps) &&
                osg::equivalent((float)worldPoint[1], m_polygonSelection->m_mouseDownY, eps))
            {
                if (!m_polygonSelection->m_gotFirstLocation)
                {
                    m_polygonSelection->m_clearCallback();
                    m_polygonSelection->m_finished = false;
                    m_polygonSelection->m_gotFirstLocation = true;
                    m_polygonSelection->m_lastPointTemporary = false;
                    m_polygonSelection->m_pushVerticeCallback(worldPoint);
                }
                else
                {
                    if (m_polygonSelection->m_lastPointTemporary)
                    {
                        m_polygonSelection->m_updateVerticeCallback(worldPoint);
                        m_polygonSelection->m_lastPointTemporary = false;
                    }
                    else
                    {
                        m_polygonSelection->m_pushVerticeCallback(worldPoint);
                    }
                    if (m_polygonSelection->m_finished)
                    {
                        m_polygonSelection->m_gotFirstLocation = false;
                    }
                }
            }
        }
        else if (ea.getEventType() == osgGA::GUIEventAdapter::MOVE)
        {
            if (m_polygonSelection->m_gotFirstLocation)
            {
                if (!m_polygonSelection->m_lastPointTemporary)
                {
                    m_polygonSelection->m_pushVerticeCallback(worldPoint);
                    m_polygonSelection->m_lastPointTemporary = true;
                }
                else
                {
                    m_polygonSelection->m_updateVerticeCallback(worldPoint);
                }
            }
        }
        else if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE &&
                 ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
        {
            if (!m_polygonSelection->m_gotFirstLocation)
            {
                return false;
			}
            if (m_polygonSelection->m_lastPointTemporary)
            {
                m_polygonSelection->m_updateVerticeCallback(worldPoint);
			}
            else
            {
                m_polygonSelection->m_pushVerticeCallback(worldPoint);
			}
            //m_polygonSelection->m_doSelectCallback();
            m_polygonSelection->m_finished = true;
            m_polygonSelection->m_gotFirstLocation = false;
		}
        else if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN &&
                 ea.getKey() == osgGA::GUIEventAdapter::KEY_BackSpace)
        {
            if (m_polygonSelection->m_lastPointTemporary)
            {
                m_polygonSelection->m_revokeCallback();
			}
		}
        return true;
    }

    return false;
}