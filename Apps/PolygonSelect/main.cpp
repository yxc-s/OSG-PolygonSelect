#include <iostream>

#include <PolygonSelectHandler.h>

int main(){
	osg::ref_ptr<osg::Group> root = new osg::Group();

	auto createHudCamera = []() {
		hudCamera = new osg::Camera;
		hudCamera->setProjectionMatrix(osg::Matrix::ortho2D(0, 640, 0, 480));
		hudCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		hudCamera->setViewMatrix(osg::Matrix::identity());
		hudCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
		hudCamera->setRenderOrder(osg::Camera::POST_RENDER);
		hudCamera->setAllowEventFocus(false);
		auto ss = hudCamera->getOrCreateStateSet();
		ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
		ss->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
		return hudCamera;
	};

	osg::Camera hudCamera = creatHudCamera();

	root->addChild(hudCamera);

	osgViewer::Viewer viewer;
	viewer.setSceneData(root);

	viewer.addEventHandler(new PolygonSelectHandler(hudCamera));



	return viewer.run();
}