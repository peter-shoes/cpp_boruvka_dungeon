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
    size_x = 16;
    size_y = 16;
    num_rooms = 5;
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
    boruvka(rooms, num_rooms);
    
    // print dungeon
    print_dungeon(dungeon, map_size.x, map_size.y);
    return 0;
    
}

int boruvka(Room rooms[], int num_rooms) {
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

        // Initialize the cheapest edge for each component to "None"
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

        for (int i=0; i<(int)E.size(); i++) {E[i].print();}

        if (!completed) {
            // NOTE: we are moving this to the end for logical reasons
            // Find the connected components of F and assign to each vertex its component

            // create id pool
            std::vector<std::vector<int>> pool;

            for (int i=0; i < (int)E.size(); i++) {

                std::cout<<"pool:"<<std::endl;
                for (const auto& row : pool) { // Iterate over each row
                    for (int num : row) { // Iterate over each element in the row
                        std::cout << num << " ";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;

                std::vector<int> sub_pool = {
                    E[i].t.id,
                    E[i].f.id
                };
                
                // in the case that this is the first pass
                if (pool.size() == 0) {
                    pool.push_back(sub_pool);
                    continue;
                }

                // otherwise get the intersection of each sub_pool
                std::vector<int> intersection_idxs;
                for (int j=0; j < (int)pool.size(); j++) {
                    
                    std::vector<int> intersection;
                    std::sort(pool[j].begin(), pool[j].end());
                    std::sort(sub_pool.begin(), sub_pool.end());
                    
                    // get the intersection
                    std::set_intersection(
                        pool[j].begin(), pool[j].end(), 
                        sub_pool.begin(), sub_pool.end(), 
                        std::back_inserter(intersection)
                    );
                    
                    // if there is an intersection, mark the sub pool
                    if (intersection.size() != 0) {intersection_idxs.push_back(j);}
                    // otherwise add a new sub_pool
                    else {pool.push_back(sub_pool);}
                }
                
                // check the intersection indicies
                if (intersection_idxs.size() > 0) {
                    std::cout<<"intersection idxs: ";
                    for (int num : intersection_idxs) {std::cout << num << " ";}std::cout << std::endl;
                    // union sub_pool with the first hit
                    std::vector<int> result(pool[intersection_idxs[0]].size() + sub_pool.size());
                    auto it = std::set_union(
                        pool[intersection_idxs[0]].begin(), pool[intersection_idxs[0]].end(), 
                        sub_pool.begin(), sub_pool.end(), 
                        result.begin());
                    result.resize(it - result.begin());
                    pool[intersection_idxs[0]] = result;
                    if (intersection_idxs.size() == 1) {continue;}
                    
                    // union all subsequent hits with the first hit
                    for(int j=1; j<(int)intersection_idxs.size(); j++) {

                        std::vector<int> result1(pool[intersection_idxs[0]].size() + pool[j].size());

                        auto it = std::set_union(
                            pool[j].begin(), pool[j].end(), 
                            pool[intersection_idxs[0]].begin(), pool[intersection_idxs[0]].end(), 
                            result1.begin());
                        result1.resize(it - result1.begin());

                        pool[intersection_idxs[0]] = result1;
                        std::vector<int> n;
                        pool[intersection_idxs[j]] = n;
                    }
                }
            }
            // remove blank elements from pool
            pool.erase(std::remove_if(pool.begin(), pool.end(), [](std::vector<int> i) {return i.size() == 0;}), pool.end());

            // assign the component id to each vertex
            for (int i=0; i < (int)pool.size(); i++) {
                int new_id = (int)pool.size()+i;
                for (int j=0; j < n_vertices; j++) {
                    if (std::find(pool[i].begin(), pool[i].end(), vertices[j].id) != pool[i].end()) {
                        vertices[j].id = new_id;
                    }
                }
            }

            // update the number of components
            num_components = (int)pool.size();

            std::cout<<"final pool:"<<std::endl;
            for (const auto& row : pool) { // Iterate over each row
                for (int num : row) { // Iterate over each element in the row
                    std::cout << num << " ";
                }
                std::cout << std::endl;
            }
            // completed = true;
        }
        
    }
    std::cout << "Boruvka complete" <<std::endl;
    return 0; 
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