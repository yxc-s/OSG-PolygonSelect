#include <iostream>

#include <PolygonSelectHandler.h>

int main(){
	osg::ref_ptr<osg::Group> root = new osg::Group();

	
	osgViewer::Viewer viewer;
	viewer.setSceneData(root);

	viewer.addEventHandler(new PolygonSelectHandler(root));

	

	return viewer.run();
}