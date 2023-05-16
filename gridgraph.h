#ifndef GRIDGRAPH_H
#define GRIDGRAPH_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class gridGraph; }
QT_END_NAMESPACE

struct Vertex
{
    int index;
    std::pair<int, int> coordinate;
    Vertex():index{}, coordinate{0,0}
    {}
};

class gridGraph : public QMainWindow
{
    Q_OBJECT

public:
    gridGraph(QWidget *parent = nullptr);
    ~gridGraph();
    void paintEvent(QPaintEvent *event);
    void representGraph();
    void bfs(int source, int initialWeight);
    std::pair<int, int> coordinates(int idx);
    void prim(int source);





private slots:

    void on_create_grid_graph_button_clicked();

    void on_show_mst_button_clicked();

    void on_add_properties_button_clicked();

private:
    std::vector<std::vector<Vertex>> graph;
    std::vector<std::vector<int>> adjacency_matrix;
    QList<QLine> lines;
    QList<QLine> mst_lines;

    bool drawFlag = false;
    bool mstFlag = false;
    bool indexingFlag = false;
    bool gravityCenterFlag = false;
    bool weightFlag = false;

    int topSize = 800;
    int leftSize = 600;
    int M;
    int N;
    int gravityCenter;
    int initialWeight;

    Ui::gridGraph *ui;
};
#endif // GRIDGRAPH_H
