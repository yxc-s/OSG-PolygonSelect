#pragma once
#include <osg/Camera>
#include <osgGA/GUIEventHandler>

struct PolygonSelectionVisualizer;
struct PolygonSelection;

class HudResizeHandler : public osgGA::GUIEventHandler {
public:
    HudResizeHandler(osg::Camera* hudCamera) : m_hudCamera(hudCamera) {}

    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) override {
        if (!m_hudCamera) return false;

        if (ea.getEventType() == osgGA::GUIEventAdapter::RESIZE) {
            int w = ea.getWindowWidth();
            int h = ea.getWindowHeight();
            m_hudCamera->setProjectionMatrix(osg::Matrix::ortho2D(0, w, 0, h));
        }
        return false;
    }

private:
    osg::observer_ptr<osg::Camera> m_hudCamera;
};

class PolygonSelectHandler : public osgGA::GUIEventHandler{
public:
    PolygonSelectHandler() = delete;
    PolygonSelectHandler(osg::Camera* hudCamera);
	~PolygonSelectHandler();

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) override;

private:
    osg::ref_ptr<osg::Camera>                m_hudCamera;
    PolygonSelection*						 m_polygonSelection;
    PolygonSelectionVisualizer*				 m_polygonSelectionVisualizer;

    bool m_isSelect;
};

