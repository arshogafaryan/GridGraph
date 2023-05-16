#include "gridgraph.h"
#include "ui_gridgraph.h"
#include <QPainter>
#include <QMessageBox>
#include <queue>
#include <vector>


gridGraph::gridGraph(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::gridGraph)
{
    ui->setupUi(this);
}

gridGraph::~gridGraph()
{
    delete ui;
}

void gridGraph::paintEvent(QPaintEvent *event)
{
    if (drawFlag)
    {
            QWidget::paintEvent(event);
            QPainter painter(this);
            QPen pen1(Qt::gray);
            pen1.setWidth(10);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setPen(pen1);
            for (int i = 0; i < N; ++i)
            {
                lines.push_back(QLine(graph[0][i].coordinate.first, graph[0][i].coordinate.second,
                                    graph[M - 1][i].coordinate.first, graph[M - 1][i].coordinate.second));
            }
            for (int i = 0; i < M; i++)
            {
                lines.push_back(QLine(graph[i][0].coordinate.first, graph[i][0].coordinate.second,
                                    graph[i][N - 1].coordinate.first, graph[i][N - 1].coordinate.second));
            }
            painter.drawLines(lines);

        if (mstFlag)
        {
            QPen pen3(Qt::darkGreen);
            pen3.setWidth(10);
            painter.setPen(pen3);

            painter.drawLines(mst_lines);
            mstFlag = false;
        }

        if (indexingFlag)
        {
            QPen pen2(Qt::white);
            painter.setPen(pen2);
            for (int i = 0; i < M; ++i)
            {
                for (int j = 0; j < N; ++j)
                {
                    if (graph[i][j].index == gravityCenter && gravityCenterFlag)
                    {
                        painter.setBrush(Qt::darkGreen);
                        painter.drawEllipse(graph[i][j].coordinate.first - 15, graph[i][j].coordinate.second - 15, 30, 30);
                        gravityCenterFlag = false;
                    }
                    else
                    {
                        painter.setBrush(Qt::darkRed);
                        painter.drawEllipse(graph[i][j].coordinate.first - 15, graph[i][j].coordinate.second - 15, 30, 30);
                    }
                    if (graph[i][j].index / 10 == 0)
                        painter.drawText(graph[i][j].coordinate.first - 4, graph[i][j].coordinate.second + 5, QString::number(graph[i][j].index));
                    else if (graph[i][j].index / 10 < 10)
                        painter.drawText(graph[i][j].coordinate.first - 7, graph[i][j].coordinate.second + 5, QString::number(graph[i][j].index));
                    else
                        painter.drawText(graph[i][j].coordinate.first - 11, graph[i][j].coordinate.second + 5, QString::number(graph[i][j].index));
                }
            }
            indexingFlag = false;
        }

        if (weightFlag)
        {
            QPen pen0(Qt::black);
            painter.setPen(pen0);
            for (int i = 0; i < adjacency_matrix.size(); ++i)
            {
                for (int j = i; j < adjacency_matrix[i].size(); ++j)
                {
                    if (adjacency_matrix[i][j] != 0)
                    {
                        int leftOffset = 0, downOffset = 0;
                        if (graph[i / N][i % N].coordinate.first == graph[j / N][j % N].coordinate.first)
                            leftOffset = 4;
                        else
                            downOffset = 4;
                        painter.drawText((graph[i / N][i % N].coordinate.first + graph[j / N][j % N].coordinate.first) / 2 - leftOffset,
                                            (graph[i / N][i % N].coordinate.second + graph[j / N][j % N].coordinate.second) / 2 + downOffset,
                                            QString::number(adjacency_matrix[i][j]));
                    }
                }
            }
            weightFlag = false;
        }

        drawFlag = false;
    }
}

bool isNumber(const QString &&str)
{
    if (str.isEmpty())
    {
        return false;
    }
    for (int i = 0; i < str.size(); ++i)
    {
        if (!str[i].isDigit())
        {
            return false;
        }
    }
    return true;
}

void gridGraph::on_create_grid_graph_button_clicked()
{
    ui->gravity_center->clear();
    ui->initial_weight->clear();
    ui->total_weight->clear();
    graph.clear();
    adjacency_matrix.clear();
    lines.clear();
    mst_lines.clear();
    if (isNumber(ui->M_size->text()) && isNumber(ui->N_size->text()) && ui->M_size->text().toInt() != 0 && ui->N_size->text().toInt() != 0)
    {
        M = ui->M_size->text().toInt();
        N = ui->N_size->text().toInt();
        graph.resize(M);

        for (unsigned int i = 0; i < graph.size(); ++i)
        {
            graph[i].resize(N);
        }
        int topOffset = topSize / (N - 1);
        int leftOffset = leftSize / (M - 1);
        int index = 0;
        for (int i = 0, YCoord = 150; i < M; i++, YCoord += leftOffset)
        {
            for (int j = 0, XCoord = 100; j < N; j++, XCoord += topOffset)
            {
                graph[i][j].index = index;
                graph[i][j].coordinate = std::make_pair(XCoord, YCoord);
                ++index;
            }
        }
        drawFlag = true;
        indexingFlag = true;
    }
    else
    {
        if (ui->M_size->text().isEmpty() || ui->N_size->text().isEmpty())
        {
            QMessageBox::warning(this, "Error", "Empty fields are not allowed");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Only positive numbers are allowed");
        }
    }
    update();
}

void gridGraph::on_add_properties_button_clicked()
{
    ui->total_weight->clear();
    if (isNumber(ui->gravity_center->text()) && isNumber(ui->initial_weight->text()))
    {
        if (ui->gravity_center->text().toInt() < N*M && ui->gravity_center->text().toInt() >= 0)
        {
            gravityCenter = ui->gravity_center->text().toInt();
            initialWeight = ui->initial_weight->text().toInt();
            drawFlag = true;
            indexingFlag = true;
            gravityCenterFlag = true;
            weightFlag = true;
        }
        else
        {
            QMessageBox::warning(this, "Error", "Such index doesn't exist");
        }
    }
    else
    {
        if (ui->gravity_center->text().isEmpty() || ui->initial_weight->text().isEmpty())
        {
            QMessageBox::warning(this, "Error", "Empty fields are not allowed");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Only positive numbers are allowed");
        }
    }
    update();
}


void gridGraph::representGraph()
{
    int num_vertices = M * N;
    adjacency_matrix.resize(num_vertices, std::vector<int>(num_vertices, 0));

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            int curr = i * N + j;
            if (i > 0)
            {
                int up = (i - 1) * N + j;
                adjacency_matrix[curr][up] = adjacency_matrix[up][curr] = -1;
            }
            if (j > 0)
            {
                int left = i * N + (j - 1);
                adjacency_matrix[curr][left] = adjacency_matrix[left][curr] = -1;
            }
            if (i < M - 1)
            {
                int down = (i + 1) * N + j;
                adjacency_matrix[curr][down] = adjacency_matrix[down][curr] = -1;
            }
            if (j < N - 1)
            {
                int right = i * N + (j + 1);
                adjacency_matrix[curr][right] = adjacency_matrix[right][curr] = -1;
            }
        }
    }
}



void gridGraph::bfs(int source, int initialWeight)
{
    int n = adjacency_matrix.size();
    std::vector<int> level(n, -1);
    std::vector<bool> visited(n, false);
    std::queue<int> q;


    visited[source] = true;
    level[source] = 1;
    q.push(source);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++) {
            if (adjacency_matrix[u][v] == -1) {
            if (!visited[v]) {
                visited[v] = true;
                level[v] = level[u] + 1;
                q.push(v);
            }
            adjacency_matrix[u][v] = initialWeight * level[u];
            adjacency_matrix[v][u] = initialWeight * level[u];
        }
        }
    }
}



int minKey(int key[], bool mstSet[], int V) {
    int min_index = -1, min_value = INT_MAX;

    for (int v = 0; v < V; v++) {
        if (mstSet[v] == false && key[v] < min_value) {
            min_value = key[v], min_index = v;
        }
    }
    if (min_index == -1)
        min_index = 0;
    return min_index;
}

void gridGraph::prim(int source)
{
    int n = adjacency_matrix.size();
    std::vector<int> dist(adjacency_matrix.size(), INT_MAX) ;
    std::vector<bool> visited(adjacency_matrix.size(), false);
    std::vector<int> parent(adjacency_matrix.size(), -1);
    std::vector<int> mst;
    int totalWeight = 0;

    dist[source] = 0;

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    pq.push(std::make_pair(dist[source], source));

    while(!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if(visited[u]) {
            continue;
        }

        visited[u] = true;

        if(parent[u] != -1) {
            mst.push_back(parent[u]);
            mst.push_back(u);
            totalWeight += adjacency_matrix[u][parent[u]];

        }

        for(int v = 0; v < n; v++) {
            if(adjacency_matrix[u][v] != 0 && !visited[v] && adjacency_matrix[u][v] < dist[v]) {
                dist[v] = adjacency_matrix[u][v];
                parent[v] = u;
                pq.push(std::make_pair(dist[v], v));
            }
        }
    }

    for(int i = 0; i < mst.size(); i+=2)
    {
        mst_lines.push_back(QLine(graph[mst[i] / N][mst[i] % N].coordinate.first, graph[mst[i] / N][mst[i] % N].coordinate.second,
                            graph[mst[i + 1] / N][mst[i + 1] % N].coordinate.first, graph[mst[i + 1] / N][mst[i + 1] % N].coordinate.second));
    }
    ui->total_weight->append("Total weight of the MST from the gravity center #" + QString::number(gravityCenter) + " is " + QString::number(totalWeight));
}



void gridGraph::on_show_mst_button_clicked()
{
    ui->total_weight->clear();
    mst_lines.clear();
    representGraph();
    bfs(gravityCenter, initialWeight);
    prim(gravityCenter);
    drawFlag = true;
    mstFlag = true;
    indexingFlag = true;
    gravityCenterFlag = true;
    weightFlag = true;
    update();
}

