#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <vector>
#include <SFML/Graphics.hpp>

namespace {

constexpr std::size_t c_window_width = 1280;
constexpr std::size_t c_window_height = 720;

constexpr std::size_t c_vertex_count = 100;
constexpr float c_vertex_radius = 2.5;

class Vertex final
{
public: // methods
    Vertex()
    {
    }

    Vertex(float x, float y, std::size_t index = 0)
        : m_point(x, y)
        , m_index(index)
    {
    }

    bool operator==(const Vertex &v) const
    {
        return m_point == v.m_point;
    }

    float x() const
    {
        return m_point.x;
    }

    float y() const
    {
        return m_point.y;
    }

    sf::Vector2f point() const
    {
        return m_point;
    }

    float index() const
    {
        return m_index;
    }

private: // fields
    sf::Vector2f m_point;
    std::size_t m_index;
};

class Edge final
{
public: // methods
    Edge()
    {
    }

    Edge(const Vertex &p1, const Vertex &p2)
        : p1(p1)
        , p2(p2)
        , is_bad(false)
    {
        calculateWeight();
    }

    Edge(const Edge &edge)
        : p1(edge.p1)
        , p2(edge.p2)
        , is_bad(false)
        , weight(edge.weight)
    {
    }

    bool operator==(const Edge &edge) const
    {
        return (p1 == edge.p1 && p2 == edge.p2) ||
               (p1 == edge.p2 && p2 == edge.p1);
    }

    bool operator<(const Edge &edge) const
    {
        return weight < edge.weight;
    }

private: // methods
    void calculateWeight()
    {
        weight = std::sqrt(
            std::pow(p1.x() - p2.x(), 2) + std::pow(p1.y() - p2.y(), 2));
    }

public: // fields
    Vertex p1;
    Vertex p2;

    bool is_bad;

    float weight;
};

class Triangle final
{
public: // methods
    Triangle(const Vertex &p1, const Vertex &p2, const Vertex &p3)
        : p1(p1)
        , p2(p2)
        , p3(p3)
        , e1(p1, p2)
        , e2(p2, p3)
        , e3(p3, p1)
        , is_bad(false)
    {
    }

    bool containsVertex(const Vertex &v)
    {
        return p1 == v || p2 == v || p3 == v;
    }

    bool circumCircleContains(const Vertex &v)
    {
        float ab = (p1.x() * p1.x()) + (p1.y() * p1.y());
        float cd = (p2.x() * p2.x()) + (p2.y() * p2.y());
        float ef = (p3.x() * p3.x()) + (p3.y() * p3.y());

        float circux =
            (ab * (p3.y() - p2.y()) + cd * (p1.y() - p3.y()) +
             ef * (p2.y() - p1.y())) /
            (p1.x() * (p3.y() - p2.y()) + p2.x() * (p1.y() - p3.y()) +
             p3.x() * (p2.y() - p1.y())) /
            2.f;

        float circuy =
            (ab * (p3.x() - p2.x()) + cd * (p1.x() - p3.x()) +
             ef * (p2.x() - p1.x())) /
            (p1.y() * (p3.x() - p2.x()) + p2.y() * (p1.x() - p3.x()) +
             p3.y() * (p2.x() - p1.x())) /
            2.f;

        float circuradius = sqrtf(
            ((p1.x() - circux) * (p1.x() - circux)) +
            ((p1.y() - circuy) * (p1.y() - circuy)));

        float dist = sqrtf(
            ((v.x() - circux) * (v.x() - circux)) +
            ((v.y() - circuy) * (v.y() - circuy)));

        return dist <= circuradius;
    }

    bool operator==(const Triangle &t)
    {
        return (p1 == t.p1 || p1 == t.p2 || p1 == t.p3) &&
               (p2 == t.p1 || p2 == t.p2 || p2 == t.p3) &&
               (p3 == t.p1 || p3 == t.p2 || p3 == t.p3);
    }

public: // fields
    Vertex p1;
    Vertex p2;
    Vertex p3;

    Edge e1;
    Edge e2;
    Edge e3;

    bool is_bad;
};

std::vector<Vertex> vertices;
std::size_t vertex_counter = 0;

void generateVertices()
{
    for (std::size_t i = 0; i < c_vertex_count; i++)
    {
        vertices.emplace_back(Vertex(
            static_cast<double>(std::rand()) / RAND_MAX * c_window_width,
            static_cast<double>(std::rand()) / RAND_MAX * c_window_height,
            vertex_counter++));
    }
}

std::vector<Triangle> triangulate(const std::vector<Vertex> &vertices)
{
    if (vertices.empty())
    {
        return {};
    }

    std::vector<Triangle> triangles;

    // Determinate the super triangle
    float minX = vertices[0].x();
    float minY = vertices[0].y();
    float maxX = minX;
    float maxY = minY;

    for (std::size_t i = 0; i < vertices.size(); ++i)
    {
        if (vertices[i].x() < minX)
            minX = vertices[i].x();
        if (vertices[i].y() < minY)
            minY = vertices[i].y();
        if (vertices[i].x() > maxX)
            maxX = vertices[i].x();
        if (vertices[i].y() > maxY)
            maxY = vertices[i].y();
    }

    float dx = maxX - minX;
    float dy = maxY - minY;
    float deltaMax = std::max(dx, dy);
    float midx = (minX + maxX) / 2.f;
    float midy = (minY + maxY) / 2.f;

    ///@ HERE!!!
    Vertex p1(midx - 20 * deltaMax, midy - deltaMax);
    Vertex p2(midx, midy + 20 * deltaMax);
    Vertex p3(midx + 20 * deltaMax, midy - deltaMax);

    // Create a list of triangles, and add the supertriangle in it
    triangles.push_back(Triangle(p1, p2, p3));

    for (auto p = std::begin(vertices); p != std::end(vertices); p++)
    {
        std::vector<Edge> polygon;

        for (auto t = std::begin(triangles); t != std::end(triangles); t++)
        {
            if (t->circumCircleContains(*p))
            {
                t->is_bad = true;
                polygon.push_back(t->e1);
                polygon.push_back(t->e2);
                polygon.push_back(t->e3);
            }
            else
            {
            }
        }

        triangles.erase(
            std::remove_if(
                begin(triangles),
                end(triangles),
                [](Triangle &t) { return t.is_bad; }),
            end(triangles));

        for (auto e1 = std::begin(polygon); e1 != std::end(polygon); e1++)
        {
            for (auto e2 = std::begin(polygon); e2 != std::end(polygon); e2++)
            {
                if (e1 == e2)
                    continue;

                if (*e1 == *e2)
                {
                    e1->is_bad = true;
                    e2->is_bad = true;
                }
            }
        }

        polygon.erase(
            std::remove_if(
                begin(polygon), end(polygon), [](Edge &e) { return e.is_bad; }),
            end(polygon));

        for (auto e = std::begin(polygon); e != std::end(polygon); e++)
            triangles.push_back(Triangle(e->p1, e->p2, *p));
    }

    triangles.erase(
        std::remove_if(
            begin(triangles),
            end(triangles),
            [p1, p2, p3](Triangle &t) {
                return t.containsVertex(p1) || t.containsVertex(p2) ||
                       t.containsVertex(p3);
            }),
        end(triangles));

    return triangles;
}

std::vector<Edge> decomposeTrianglesIntoEdges(
    const std::vector<Triangle> &triangles)
{
    std::vector<Edge> edges;

    for (auto t = begin(triangles); t != end(triangles); t++)
    {
        edges.push_back(t->e1);
        edges.push_back(t->e2);
        edges.push_back(t->e3);
    }

    return edges;
}

class DisjointSets final
{
public: // methods
    explicit DisjointSets(int n)
        : n{n}
    {
        parent.resize(n + 1);
        rnk.resize(n + 1);

        std::fill(rnk.begin(), rnk.end(), 0);
        std::iota(parent.begin(), parent.end(), 0);
    }

    // Find the parent of a node 'u'
    // Path Compression
    int find(int u)
    {
        /* Make the parent of the nodes in the path
           from u--> parent[u] point to parent[u] */
        if (u != parent[u])
        {
            parent[u] = find(parent[u]);
        }
        return parent[u];
    }

    // Union by rank
    void merge(int x, int y)
    {
        x = find(x);
        y = find(y);

        /* Make tree with smaller height
           a subtree of the other tree  */
        if (rnk[x] > rnk[y])
            parent[y] = x;
        else // If rnk[x] <= rnk[y]
            parent[x] = y;

        if (rnk[x] == rnk[y])
            rnk[y]++;
    }

private: // fields
    std::vector<int> parent;
    std::vector<int> rnk;

    int n;
};

std::vector<Edge> findEuclideanMinimalSpanningTree(
    const std::vector<Edge> &unsorted_edges)
{
    std::vector<Edge> edges = unsorted_edges;

    // Sort edges in increasing order on basis of cost
    std::sort(edges.begin(), edges.end());

    // Create disjoint sets
    DisjointSets ds(vertices.size());

    std::vector<Edge> tree;

    // Iterate through all sorted edges
    for (const Edge &edge : edges)
    {
        int u = edge.p1.index();
        int v = edge.p2.index();

        int set_u = ds.find(u);
        int set_v = ds.find(v);

        // Check if the selected edge is creating
        // a cycle or not (Cycle is created if u
        // and v belong to same set)
        if (set_u != set_v)
        {
            tree.push_back(edge);

            // Merge two sets
            ds.merge(set_u, set_v);
        }
    }

    return tree;
}

} // namespace

int main()
{
    std::srand(std::time(nullptr));

    sf::ContextSettings context_settings;
    context_settings.antialiasingLevel = 8;

    sf::VideoMode video_mode{c_window_width, c_window_height};
    sf::RenderWindow window(
        video_mode, "EMST", sf::Style::Default, context_settings);

    sf::CircleShape vertex_shape(c_vertex_radius);
    vertex_shape.setFillColor(sf::Color::Red);
    vertex_shape.setOutlineThickness(0.75);
    vertex_shape.setOutlineColor(sf::Color::Black);

    std::vector<sf::Vertex> edges_vertex_buffer;

    window.setPosition(sf::Vector2i(
        (sf::VideoMode::getDesktopMode().width - video_mode.width) / 2,
        (sf::VideoMode::getDesktopMode().height - video_mode.height) / 2));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::Space)
                {
                    generateVertices();
                }
                else if (event.key.code == sf::Keyboard::G)
                {
                    edges_vertex_buffer.clear();

                    std::vector<Triangle> triangles = triangulate(vertices);

                    std::vector<Edge> edges =
                        decomposeTrianglesIntoEdges(triangles);

                    sf::Color graph_color(175, 175, 175);

                    for (const Edge &edge : edges)
                    {
                        edges_vertex_buffer.emplace_back(
                            sf::Vertex(edge.p1.point()));
                        edges_vertex_buffer.back().color = graph_color;
                        edges_vertex_buffer.emplace_back(
                            sf::Vertex(edge.p2.point()));
                        edges_vertex_buffer.back().color = graph_color;
                    }

                    std::vector<Edge> emst_edges =
                        findEuclideanMinimalSpanningTree(edges);

                    for (const Edge &edge : emst_edges)
                    {
                        edges_vertex_buffer.emplace_back(
                            sf::Vertex(edge.p1.point()));
                        edges_vertex_buffer.back().color = sf::Color::Black;
                        edges_vertex_buffer.emplace_back(
                            sf::Vertex(edge.p2.point()));
                        edges_vertex_buffer.back().color = sf::Color::Black;
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    vertices.emplace_back(Vertex(
                        static_cast<float>(event.mouseButton.x) /
                            window.getSize().x * c_window_width,
                        static_cast<float>(event.mouseButton.y) /
                            window.getSize().y * c_window_height,
                        vertex_counter++));
                }
            }
        }

        window.clear(sf::Color{200, 200, 200});

        window.draw(
            edges_vertex_buffer.data(), edges_vertex_buffer.size(), sf::Lines);

        for (const Vertex &vertex : vertices)
        {
            vertex_shape.setPosition(
                vertex.point() -
                sf::Vector2f(c_vertex_radius, c_vertex_radius));
            window.draw(vertex_shape);
        }

        window.display();
    }

    return 0;
}
