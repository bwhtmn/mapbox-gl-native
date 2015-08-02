#include "../platform/default/default_styles.hpp"

#include <mbgl/platform/qt/QMapboxGL>

#include <QApplication>
#include <QGLContext>
#include <QGLFormat>
#include <QGLWidget>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QString>

class MapWindow : public QGLWidget {
public:
    MapWindow(QGLContext *ctx)
        : QGLWidget(ctx) // QGLWidget takes the ownership of ctx.
        , m_map(ctx)
    {
        m_map.setAccessToken(qgetenv("MAPBOX_ACCESS_TOKEN"));
        changeStyle();
    }

private:
    void changeStyle()
    {
        static uint8_t currentStyleIndex;

        const auto& newStyle = mbgl::util::defaultStyles[currentStyleIndex];
        QString url(newStyle.first.c_str());

        m_map.setStyleURL(url);

        QString name(newStyle.second.c_str());
        setWindowTitle(QString("Mapbox GL: ") + name);

        if (++currentStyleIndex == mbgl::util::defaultStyles.size()) {
            currentStyleIndex = 0;
        }
    }

    void keyPressEvent(QKeyEvent *ev) final
    {
        if (ev->key() == Qt::Key_S) {
            changeStyle();
        }

        ev->accept();
    }

    void mousePressEvent(QMouseEvent *ev) final
    {
        lastX = ev->x();
        lastY = ev->y();

        if (ev->type() == QEvent::MouseButtonDblClick) {
            if (ev->buttons() == Qt::LeftButton) {
                m_map.scaleBy(2.0, lastX, lastY);
            } else if (ev->buttons() == Qt::RightButton) {
                m_map.scaleBy(0.5, lastX, lastY);
            }
        }

        ev->accept();
    }

    void mouseMoveEvent(QMouseEvent *ev) final
    {
        if (!(ev->buttons() & Qt::LeftButton)) {
            return;
        }

        int dx = ev->x() - lastX;
        int dy = ev->y() - lastY;

        lastX = ev->x();
        lastY = ev->y();

        if (dx || dy) {
            m_map.moveBy(dx, dy);
        }

        ev->accept();
    }

    void wheelEvent(QWheelEvent *ev) final
    {
        if (ev->orientation() == Qt::Horizontal) {
            return;
        }

        float factor = ev->delta() / 1200.;
        if (ev->delta() < 0) {
            factor = factor > -1 ? factor : 1 / factor;
        }

        m_map.scaleBy(1 + factor, ev->x(), ev->y());
        ev->accept();
    }

    void resizeGL(int w, int h) final
    {
        m_map.resize(w, h);
    }

    int lastX = 0;
    int lastY = 0;

    QMapboxGL m_map;
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QGLContext *context = new QGLContext(QGLFormat::defaultFormat());

    // MapWindow inherits from QGLWidget and will get
    // the ownership of the QGLContext while internally
    // it will create a QMapboxGL that will use the
    // context for rendering. QGLWidget will also handle
    // the input events and forward them to QMapboxGL
    // if necessary.
    MapWindow view(context);

    view.resize(800, 600);
    view.setFocus();
    view.show();

    return app.exec();
}
