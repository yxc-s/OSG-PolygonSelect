#include "PolygonSelectHandler.h"
#include <osg/Camera>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>


int main(){
	osg::ref_ptr<osg::Group> root = new osg::Group();

	auto createHudCamera = []() {
		auto hudCamera = new osg::Camera;
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

	osgViewer::Viewer viewer;
	auto manipulator = new osgGA::TrackballManipulator(); /* 建一个 Trackball 类型的摄像机操纵器 */
	auto standardManipulator = (osgGA::StandardManipulator*)manipulator; /* Trackball 是 StandardManipulator 的派生类) */
	standardManipulator->setAllowThrow(false);     /* 鼠标释放后相机不会继续自动旋转 */
	viewer.setCameraManipulator(manipulator);
	viewer.getCamera()->setClearColor(
		osg::Vec4(0.9529411764705882, 0.9529411764705882, 0.9529411764705882,
			1.0));

	osg::Camera* hudCamera = createHudCamera();
	root->addChild(hudCamera);
	viewer.setSceneData(root);
	
	viewer.addEventHandler(new PolygonSelectHandler(hudCamera));
	viewer.addEventHandler(new HudResizeHandler(hudCamera));

	viewer.setUpViewInWindow(100, 100, 800, 600); // (x, y, width, height)
	return viewer.run();
}