#include <osg/Geode>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osgViewer/Viewer>
#include <osg/NodeCallback>

class RotateCallback : public osg::NodeCallback
{
public:
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        osg::Timer_t now = osg::Timer::instance()->tick();

        float angle = osg::Timer::instance()->delta_s(lastTick_, now) * 60.0f; // 60 grados por segundo

        osg::Matrix rotation;
        rotation.makeRotate(osg::DegreesToRadians(angle), osg::Vec3(2.0f, 1.0f, 1.0f));  // Girar cubo en los 3 ejes 

        osg::MatrixTransform* transform = dynamic_cast<osg::MatrixTransform*>(node);
        if (transform)
        {
            osg::Matrix matrix = transform->getMatrix();
            matrix.preMult(rotation);
            transform->setMatrix(matrix);
        }

        lastTick_ = now;

        traverse(node, nv);
    }

private:
    osg::Timer_t lastTick_ = osg::Timer::instance()->tick();
};

int main()
{
    // Create a Geode (a node that can hold drawable objects)
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();

    // Create a Geometry (a node that holds vertices and other geometry data)
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

    // Create a Vec3Array to hold the vertices of the cube
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(-1.0f, -1.0f, -1.0f));
    vertices->push_back(osg::Vec3(1.0f, -1.0f, -1.0f));
    vertices->push_back(osg::Vec3(1.0f, 1.0f, -1.0f));
    vertices->push_back(osg::Vec3(-1.0f, 1.0f, -1.0f));
    vertices->push_back(osg::Vec3(-1.0f, -1.0f, 1.0f));
    vertices->push_back(osg::Vec3(1.0f, -1.0f, 1.0f));
    vertices->push_back(osg::Vec3(1.0f, 1.0f, 1.0f));
    vertices->push_back(osg::Vec3(-1.0f, 1.0f, 1.0f));

    // Associate the vertices with the geometry
    geom->setVertexArray(vertices);

    // Define the faces (quads) of the cube
    osg::ref_ptr<osg::DrawElementsUInt> quad =
        new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
    quad->push_back(0); quad->push_back(1); quad->push_back(2); quad->push_back(3); // front
    quad->push_back(4); quad->push_back(5); quad->push_back(6); quad->push_back(7); // back
    quad->push_back(0); quad->push_back(1); quad->push_back(5); quad->push_back(4); // bottom
    quad->push_back(2); quad->push_back(3); quad->push_back(7); quad->push_back(6); // top
    quad->push_back(0); quad->push_back(3); quad->push_back(7); quad->push_back(4); // left
    quad->push_back(1); quad->push_back(2); quad->push_back(6); quad->push_back(5); // right
    geom->addPrimitiveSet(quad);

    // Associate colors with the vertices
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f)); // red
    colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f)); // green
    colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f)); // blue
    colors->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f)); // yellow
    colors->push_back(osg::Vec4(1.0f, 0.0f, 1.0f, 1.0f)); // magenta
    colors->push_back(osg::Vec4(0.0f, 1.0f, 1.0f, 1.0f)); // cyan
    geom->setColorArray(colors, osg::Array::BIND_PER_VERTEX);

    // Add the geometry to the geode
    geode->addDrawable(geom);

    // Create a MatrixTransform node to hold the Geode and apply transformations
    osg::ref_ptr<osg::MatrixTransform> root = new osg::MatrixTransform;
    root->addChild(geode);

    // Add a rotation callback to continuously rotate the cube
    root->setUpdateCallback(new RotateCallback);

    // Create the viewer
    osgViewer::Viewer viewer;
    viewer.setSceneData(root);

    // Run the viewer loop
    return viewer.run();
}
