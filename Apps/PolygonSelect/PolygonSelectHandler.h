#pragma once

struct PolygonSelectionVisualizer;
struct PolygonSelection;

class PolygonSelectHandler : public osgGA::GUIEventHandler{
public:
    PolygonSelectHandler() = delete;
    PolygonSelectHandler(osg::ref_ptr root);
    ~PolygonSelectHandler();

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) override;

private:
    osg::ref_ptr<osg::Group>                 m_root;
    osg::ref_ptr<PolygonSelection>           m_polygonSelection;
    osg::ref_ptr<PolygonSelectionVisualizer> m_polygonSelectionVisualizer;

    bool m_isSelect;
};

