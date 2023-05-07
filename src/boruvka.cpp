#include "../inc/boruvka.hpp"

using namespace std;

int main() {
    int size_x;
    int size_y;
    int num_rooms;
    // // ask user for size of dungeon
    // std::cout << "size_x: ";
    // std::cin >> size_x;
    // std::cout << "size_y: ";
    // std::cin >> size_y;
    // // ask user for number of rooms
    // std::cout << "Enter number of rooms: ";
    // std::cin >> num_rooms;
    size_x = 32;
    size_y = 32;
    num_rooms = 12;
    Vector2 map_size = Vector2(size_x, size_y);
    Vector2 room_size = Vector2(5, 5);
    // random seed
    srand(time(NULL));
    // generate dungeon
    generate(map_size, num_rooms, room_size);
    return 0;
}

int generate(Vector2 map_size, 
            int num_rooms, 
            Vector2 room_size) {
    
    // create 2d array of zeros of size_x by size_y
    int **dungeon = new int*[map_size.x];
    for (int i = 0; i < map_size.x; i++) {
        dungeon[i] = new int[map_size.y];
        for(int j = 0; j < map_size.y; j++) {
            dungeon[i][j] = 0;
        }
    }

    // create rooms
    // TODO: this needs a timeout
    // create empty room array
    Room rooms[num_rooms];
    for (int i=0; i < num_rooms; i++) {
        bool solved = false;
        while (!solved) {
            int sx = rand() % room_size.x +1+2;
            int sy = rand() % room_size.y +1+2;
            int lx = rand() % map_size.x;
            int ly = rand() % map_size.y;
            // check if room is in bounds
            if (sx+lx < map_size.x && sy+ly < map_size.y) {
                // check if room overlaps with another room
                bool overlap = false;
                for (int j = lx-1; j < sx+lx+1; j++) {
                    for (int k = ly-1; k < sy+ly+1; k++) {
                        if (j < 0 || j >= map_size.x || k < 0 || k >= map_size.y) {
                            continue;
                        }
                        if (dungeon[j][k] == 1) {
                            overlap = true;
                        }                
                    }
                }
                // if room does not overlap, add it to the dungeon
                if (!overlap) {
                    // TODO: make Room object and add to array
                    Room room = Room(lx, ly, sx, sy);
                    // Room(lx, ly, sx, sy);
                    rooms[i] = room;
                    // append to room array
                    solved = true;
                    for (int j = lx; j < sx+lx; j++) {
                        for (int k = ly; k < sy+ly; k++) {
                            dungeon[j][k] = 1;
                        }
                    }
                }
                
            }
    
        }
    }

    // do boruvka
    std::vector<Edge> edges = boruvka(rooms, num_rooms);

    // do a*
    do_a_star(dungeon, map_size.x, map_size.y, edges);

    
    // print dungeon
    print_dungeon(dungeon, map_size.x, map_size.y);
    return 0;
    
}

std::vector<Edge> boruvka(Room rooms[], int num_rooms) {
    // create vertex array
    int n_vertices = 0;
    for (int i = 0; i < num_rooms; i++) {
        n_vertices += rooms[i].sx * 2;
        n_vertices += rooms[i].sy * 2 - 4;
    }
    Vertex vertices[n_vertices];
    int v_index = 0;
    for (int i = 0; i < num_rooms; i++) {
        for (int j = 0; j < rooms[i].sx; j++) {
            vertices[v_index] = Vertex(rooms[i].lx+j, rooms[i].ly, i);
            v_index++;
            vertices[v_index] = Vertex(rooms[i].lx+j, rooms[i].ly+rooms[i].sy-1, i);
            v_index++;
        }
    }
    /*
    algorithm Borůvka is
    input: A weighted undirected graph G = (V, E).
    output: F, a minimum spanning forest of G.

    Initialize a forest F to (V, E') where E' = {}.
    */
    std::vector<Edge> E;
    int num_components = num_rooms;
    
    // completed := false
    bool completed = false;
    // while not completed do
    while(!completed) {

        // Assign each vertex it's component id
        if (!completed and E.size() != 0) {
            std::map<int, int> map;
            for (int i=0; i < num_components; i++) {
                map.insert(std::make_pair(i, -1));
            }

            // sort E by t id
            std::sort(E.begin(), E.end(), [](const Edge& lhs, const Edge& rhs) {
                return lhs.t.id < rhs.t.id;
            });

            for (int i=0; i < (int)E.size(); i++) {
                map[E[i].t.id] = E[i].f.id;
            }
            // sort map by key
            std::map<int, int>::iterator it = map.begin();
            std::vector<int> keys;
            std::vector<int> values;
            while (it != map.end()) {
                keys.push_back(it->first);
                values.push_back(it->second);
                it++;
            }
            std::sort(keys.begin(), keys.end());
            std::sort(values.begin(), values.end());

            // print map
            for (int i=0; i < (int)keys.size(); i++) {
                std::cout << keys[i] << " " << values[i] << std::endl;
            }

            bool done = false;
            while (!done){
                done = true;
                for (int i=0; i<num_components; i++) {
                    for (int j=0; j<n_vertices; j++) {
                        if (i == vertices[j].id && map[i] != -1) {
                            vertices[j].id = map[i];
                            done = false;
                        }
                    }
                }
            }
            // sort vertices by component id
            std::sort(vertices, vertices+n_vertices, [](const Vertex& lhs, const Vertex& rhs) {
                return lhs.id < rhs.id;
            });
            vector <int> v;
            for (int i=0; i<n_vertices; i++) {
                // check that vertex id is not already in v
                bool in_v = false;
                for (int j=0; j<(int)v.size(); j++) {
                    if (vertices[i].id == v[j]) {
                        in_v = true;
                    }
                }
                if (!in_v) {
                    v.push_back(vertices[i].id);
                }
            }
            if ((int)v.size() == num_components) {
                completed = true;
                break;
            }
            else{
                num_components = v.size();
            }
        }

        // Initialize the cheapest edge for each component to "None"
        std::cout<<"num_components: "<<num_components<<"\n";
        Component components[num_components];

        // for each edge uv in E, where u and v are in different components of F:
        for (int u=0; u < n_vertices; u++) {
            for (int v=0; v < n_vertices; v++) {
                if (vertices[u].id == vertices[v].id) {continue;}
                // Make edge uv
                Edge uv = Edge(vertices[u], vertices[v]);
                
                // let wx be the cheapest edge for the component of u
                Edge wx = components[vertices[u].id].cheapest_edge;

                // if is-preferred-over(uv, wx) then
                if (is_preferred_over(uv, wx)) {
                    // Set uv as the cheapest edge for the component of u
                    components[vertices[u].id].cheapest_edge = uv;
                }
                
                // let yz be the cheapest edge for the component of v
                Edge yz = components[vertices[v].id].cheapest_edge;
                // if is-preferred-over(uv, yz) then
                if (is_preferred_over(uv, yz)) {
                    // Set uv as the cheapest edge for the component of v
                    components[vertices[v].id].cheapest_edge = uv;
                }
            }
        }
        // if all components have cheapest edge set to "None" then
        // no more trees can be merged -- we are finished
        // completed := true
        completed = true;
        for (int i=0; i<num_components; i++) {
            // std::cout<<components[i].cheapest_edge.weight<<std::endl;
            if (components[i].id == -1) {continue;}
            if (components[i].cheapest_edge.weight != -1) {
                completed = false;
                E.push_back(components[i].cheapest_edge);
            }
        }
        // Sort the vector
        std::sort(E.begin(), E.end());

        // remove duplicate edges from E (if any)
        E.erase(std::unique(E.begin(), E.end()), E.end());

    }
    std::cout << "Boruvka complete" <<std::endl;
    for (int i=0; i<n_vertices; i++) {
                vertices[i].print();}
    return E; 
}

bool is_preferred_over(Edge uv, Edge wx) {
    if ((wx.weight == -1) | (uv.weight < wx.weight)) {
        return true;
    }
    return false;
}

void print_dungeon(int** array, int rows, int cols) {
  // Access elements of the array using the [] operator
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
        int val = array[i][j];
        if (val == 0) {
            std::cout << "░";
        } else {
            std::cout << "▓";
        }
    }
    std::cout << std::endl;
  }
}

void do_a_star(int** dungeon, int rows, int cols, std::vector<Edge> edges) {
    for (int i=0; i < (int) edges.size(); i++) {
        Vector2 start = Vector2(edges[i].t.x, edges[i].t.y);
        Vector2 end = Vector2(edges[i].f.x, edges[i].f.y);

        while(start.x != end.x) {
            if (start.x < end.x) {start.x++;}
            else {start.x--;}
            dungeon[start.x][start.y] = 1;
        }
        while(start.y != end.y) {
            if (start.y < end.y) {start.y++;}
            else {start.y--;}
            dungeon[start.x][start.y] = 1;
        }
    }
}