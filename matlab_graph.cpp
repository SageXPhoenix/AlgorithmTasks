#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <algorithm>

class Vertex {
public:
    virtual ~Vertex() = default;
    virtual int num_inputs() const = 0;
    virtual int num_outputs() const = 0;
    virtual void set_input(int inp_idx, double inp_val) = 0;
    virtual double get_output(int out_idx) = 0;
    virtual void calc_value() = 0;
};

struct Edge {
    int out_vertex_id;
    int out_port_id;
    int inp_vertex_id;
    int inp_port_id;
    
    Edge(int out_v, int out_p, int inp_v, int inp_p)
        : out_vertex_id(out_v), out_port_id(out_p), 
          inp_vertex_id(inp_v), inp_port_id(inp_p) {}
};

class PlusOperator : public Vertex {
    std::vector<double> m_inp_val;
    double m_out_val = 0;
    
public:
    PlusOperator() {
        m_inp_val.resize(2, 0);
    }
    
    int num_inputs() const override {
        return 2;
    }
    
    int num_outputs() const override {
        return 1;
    }
    
    void set_input(int inp_idx, double inp_val) override {
        m_inp_val[inp_idx] = inp_val;
    }
    
    void calc_value() override {
        m_out_val = m_inp_val[0] + m_inp_val[1];
    }
    
    double get_output(int out_idx) override {
        return m_out_val;
    }
};

class MultiplyOperator : public Vertex {
    std::vector<double> m_inp_val;
    double m_out_val = 0;
    
public:
    MultiplyOperator() {
        m_inp_val.resize(2, 0);
    }
    
    int num_inputs() const override {
        return 2;
    }
    
    int num_outputs() const override {
        return 1;
    }
    
    void set_input(int inp_idx, double inp_val) override {
        m_inp_val[inp_idx] = inp_val;
    }
    
    void calc_value() override {
        m_out_val = m_inp_val[0] * m_inp_val[1];
    }
    
    double get_output(int out_idx) override {
        return m_out_val;
    }
};

class Constant : public Vertex {
    double m_value;
    
public:
    Constant(double value = 0) : m_value(value) {}
    
    int num_inputs() const override {
        return 0;
    }
    
    int num_outputs() const override {
        return 1;
    }
    
    void set_input(int inp_idx, double inp_val) override {
    }
    
    void calc_value() override {
    }
    
    double get_output(int out_idx) override {
        return m_value;
    }
};

class CalcGraph : public Vertex {
private:
    std::vector<std::unique_ptr<Vertex>> vertices;
    std::vector<Edge> edges;
    std::vector<int> topological_order;
    std::vector<std::pair<int, int>> free_inputs;
    std::vector<std::pair<int, int>> free_outputs;
    std::vector<std::vector<int>> adjacency_list;
    std::vector<int> in_degree;
    bool data_set = false;
    
    void build_graph() {
        adjacency_list.resize(vertices.size());
        in_degree.resize(vertices.size(), 0);
        
        for (const auto& edge : edges) {
            adjacency_list[edge.out_vertex_id].push_back(edge.inp_vertex_id);
            in_degree[edge.inp_vertex_id]++;
        }
    }
    
    void topological_sort() {
        std::vector<int> degree = in_degree;
        std::queue<int> q;
        
        for (size_t i = 0; i < vertices.size(); ++i) {
            if (degree[i] == 0) {
                q.push(i);
            }
        }
        
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            topological_order.push_back(current);
            
            for (int neighbor : adjacency_list[current]) {
                degree[neighbor]--;
                if (degree[neighbor] == 0) {
                    q.push(neighbor);
                }
            }
        }
    }
    
    void find_free_ports() {
        std::vector<std::vector<bool>> input_connected(vertices.size());
        std::vector<std::vector<bool>> output_connected(vertices.size());
        
        for (size_t i = 0; i < vertices.size(); ++i) {
            input_connected[i].resize(vertices[i]->num_inputs(), false);
            output_connected[i].resize(vertices[i]->num_outputs(), false);
        }
        
        for (const auto& edge : edges) {
            input_connected[edge.inp_vertex_id][edge.inp_port_id] = true;
            output_connected[edge.out_vertex_id][edge.out_port_id] = true;
        }
        
        for (size_t i = 0; i < vertices.size(); ++i) {
            for (int j = 0; j < vertices[i]->num_inputs(); ++j) {
                if (!input_connected[i][j]) {
                    free_inputs.emplace_back(i, j);
                }
            }
            for (int j = 0; j < vertices[i]->num_outputs(); ++j) {
                if (!output_connected[i][j]) {
                    free_outputs.emplace_back(i, j);
                }
            }
        }
    }
    
    double get_output_value(int vertex_id, int port_id) {
        return vertices[vertex_id]->get_output(port_id);
    }
    
    void set_input_value(int vertex_id, int port_id, double value) {
        vertices[vertex_id]->set_input(port_id, value);
    }

public:
    void set_data(const std::vector<Vertex*>& vertex_ptrs, const std::vector<Edge>& new_edges) {
        vertices.clear();
        edges = new_edges;
        
        for (auto* ptr : vertex_ptrs) {
            vertices.emplace_back(ptr);
        }
        
        build_graph();
        topological_sort();
        find_free_ports();
        
        data_set = true;
    }
    
    int num_inputs() const override {
        return free_inputs.size();
    }
    
    int num_outputs() const override {
        return free_outputs.size();
    }
    
    void set_input(int inp_idx, double inp_val) override {
        auto [vertex_id, port_id] = free_inputs[inp_idx];
        set_input_value(vertex_id, port_id, inp_val);
    }
    
    double get_output(int out_idx) override {
        auto [vertex_id, port_id] = free_outputs[out_idx];
        return get_output_value(vertex_id, port_id);
    }
    
    void calc_value() override {
        for (const auto& edge : edges) {
            double output_value = get_output_value(edge.out_vertex_id, edge.out_port_id);
            set_input_value(edge.inp_vertex_id, edge.inp_port_id, output_value);
        }
        
        for (int vertex_id : topological_order) {
            vertices[vertex_id]->calc_value();
        }
    }
};

int main() {
    CalcGraph cg;
    std::vector<Vertex*> vertices;
    std::vector<Edge> edges;
    
    vertices.push_back(new PlusOperator());
    vertices.push_back(new PlusOperator());
    
    edges.emplace_back(0, 0, 1, 0);
    
    cg.set_data(vertices, edges);
    
    std::cout << cg.num_inputs() << std::endl;
    std::cout << cg.num_outputs() << std::endl;
    
    cg.set_input(0, 1.0);
    cg.set_input(1, 2.0);
    cg.set_input(2, 3.0);
    
    cg.calc_value();
    std::cout << cg.get_output(0) << std::endl;
    
    return 0;
}
