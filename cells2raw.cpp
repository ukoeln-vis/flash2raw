#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

struct vec3i { int x,y,z; };
inline vec3i operator+(vec3i a, vec3i b)
{ return {a.x+b.x,a.y+b.y,a.z+b.z}; }
inline vec3i operator-(vec3i a, vec3i b)
{ return {a.x-b.x,a.y-b.y,a.z-b.z}; }
inline vec3i operator*(vec3i a, vec3i b)
{ return {a.x*b.x,a.y*b.y,a.z*b.z}; }
inline vec3i operator/(vec3i a, vec3i b)
{ return {a.x/b.x,a.y/b.y,a.z/b.z}; }

inline vec3i operator+(vec3i a, int b)
{ return {a.x+b,a.y+b,a.z+b}; }
inline vec3i operator-(vec3i a, int b)
{ return {a.x-b,a.y-b,a.z-b}; }
inline vec3i operator*(vec3i a, int b)
{ return {a.x*b,a.y*b,a.z*b}; }
inline vec3i operator/(vec3i a, int b)
{ return {a.x/b,a.y/b,a.z/b}; }

inline std::ostream &operator<<(std::ostream &out, vec3i v)
{ out << '(' << v.x << ',' << v.y << ',' << v.z << ')'; return out; }

struct vec3f { float x,y,z; };
inline vec3f operator+(vec3f a, vec3f b)
{ return {a.x+b.x,a.y+b.y,a.z+b.z}; }
inline vec3f operator-(vec3f a, vec3f b)
{ return {a.x-b.x,a.y-b.y,a.z-b.z}; }
inline vec3f operator*(vec3f a, vec3f b)
{ return {a.x*b.x,a.y*b.y,a.z*b.z}; }
inline vec3f operator/(vec3f a, vec3f b)
{ return {a.x/b.x,a.y/b.y,a.z/b.z}; }

inline vec3f operator+(vec3f a, float b)
{ return {a.x+b,a.y+b,a.z+b}; }
inline vec3f operator-(vec3f a, float b)
{ return {a.x-b,a.y-b,a.z-b}; }
inline vec3f operator*(vec3f a, float b)
{ return {a.x*b,a.y*b,a.z*b}; }
inline vec3f operator/(vec3f a, float b)
{ return {a.x/b,a.y/b,a.z/b}; }

inline std::ostream &operator<<(std::ostream &out, vec3f v)
{ out << '(' << v.x << ',' << v.y << ',' << v.z << ')'; return out; }

inline vec3i make_vec3i(vec3f v)
{ return vec3i{v.x,v.y,v.z}; }
inline vec3i make_vec3i(int v)
{ return vec3i{v,v,v}; }

inline vec3f make_vec3f(vec3i v)
{ return vec3f{v.x,v.y,v.z}; }
inline vec3f make_vec3f(int v)
{ return vec3f{v,v,v}; }

struct box3f { vec3f lower,upper; };

struct Cell {
    vec3i pos;
    int level;
};

inline vec3i clamp(vec3i v, vec3i a, vec3i b)
{
  return {
    std::max(a.x,std::min(b.x,v.x)),
    std::max(a.y,std::min(b.y,v.y)),
    std::max(a.z,std::min(b.z,v.z))
  };
}

inline
size_t linearIndex(const vec3i index, const vec3i dims)
{
  return index.z * size_t(dims.x) * dims.y
       + index.y * dims.x
       + index.x;
}

inline
vec3i gridIndex(const size_t index, const vec3i grid)
{
  const auto stride_y = (size_t)grid.x;
  const auto stride_z = (size_t)grid.y * (size_t)grid.x;
  return  vec3i{index % stride_y, (index % stride_z) / stride_y, index / stride_z};
}

inline
vec3i projectOnGrid(vec3f V,
                    vec3i dims,
                    box3f worldBounds)
{
  const vec3f V01 = (V-worldBounds.lower)/(worldBounds.upper-worldBounds.lower);
  return clamp(make_vec3i(V01*vec3f{dims.x,dims.y,dims.z}),vec3i{0,0,0},dims-vec3i{1,1,1});
}

int main(int argc, char **argv) {

    std::vector<float> scalars;
    std::vector<Cell> cells;

    std::string scalarFileName(argv[1]);
    std::string cellFileName(argv[2]);

    std::ifstream scalarFile(scalarFileName, std::ios::binary | std::ios::ate);
    if (scalarFile.good()) {
        size_t numBytes = scalarFile.tellg();
        scalarFile.close();
        scalarFile.open(scalarFileName, std::ios::binary);
        if (scalarFile.good()) {
            scalars.resize(numBytes/sizeof(float));
            scalarFile.read((char *)scalars.data(),scalars.size()*sizeof(float));
        }
    }

    std::ifstream cellFile(cellFileName, std::ios::binary | std::ios::ate);
    if (cellFile.good()) {
        size_t numBytes = cellFile.tellg();
        cellFile.close();
        cellFile.open(cellFileName, std::ios::binary);
        if (cellFile.good()) {
            cells.resize(numBytes/sizeof(Cell));
            cellFile.read((char *)cells.data(),cells.size()*sizeof(Cell));
        }
    }

    vec3i dims{256,256,256};
    box3f worldBounds{{1e30f,1e30f,1e30f},{-1e30f,-1e30f,-1e30f}};


    for (size_t i=0; i<cells.size(); ++i) {
        const Cell &cell = cells[i];
        box3f cellBounds;
        cellBounds.lower = make_vec3f(cell.pos);
        cellBounds.upper = make_vec3f(cells[i].pos+make_vec3i(1<<cells[i].level));

        worldBounds.lower.x = std::min(worldBounds.lower.x,cellBounds.lower.x);
        worldBounds.lower.y = std::min(worldBounds.lower.y,cellBounds.lower.y);
        worldBounds.lower.z = std::min(worldBounds.lower.z,cellBounds.lower.z);

        worldBounds.upper.x = std::max(worldBounds.upper.x,cellBounds.upper.x);
        worldBounds.upper.y = std::max(worldBounds.upper.y,cellBounds.upper.y);
        worldBounds.upper.z = std::max(worldBounds.upper.z,cellBounds.upper.z);
    }

    std::cout << worldBounds.lower << ',' << worldBounds.upper << '\n';
    std::cout << worldBounds.upper-worldBounds.lower << '\n';

    std::vector<float> grid(dims.x*size_t(dims.y)*dims.z,0.f);
    std::vector<int> updateCounts(dims.x*size_t(dims.y)*dims.z,0);

    for (size_t i=0; i<cells.size(); ++i) {
        const Cell &cell = cells[i];
        const float S = scalars[i];
        vec3i lower = cell.pos;

        vec3i upper = lower + (1<<cell.level);

        const vec3f halfCell = make_vec3f(1<<cell.level)*.5f;

        const vec3i loMC = projectOnGrid(make_vec3f(lower)-halfCell,dims,worldBounds);
        const vec3i upMC = projectOnGrid(make_vec3f(upper)+halfCell,dims,worldBounds);

        for (int mcz=loMC.z; mcz<=upMC.z; ++mcz) {
            for (int mcy=loMC.y; mcy<=upMC.y; ++mcy) {
                for (int mcx=loMC.x; mcx<=upMC.x; ++mcx) {
                    const vec3i mcID{mcx,mcy,mcz};
                    const size_t mcIndex = linearIndex(mcID,dims);
                    grid[mcIndex] += S;
                    updateCounts[mcIndex]++;
                }
            }
        }
    }

    float minVal = 1e30f, maxVal = -1e30f;
    for (size_t i=0; i<dims.x*size_t(dims.y)*dims.z; ++i) {
        if (updateCounts[i] > 0) {
            grid[i] /= updateCounts[i];
            minVal = std::min(minVal,grid[i]);
            maxVal = std::max(maxVal,grid[i]);
        }
    }

    std::cout << minVal << ' ' << maxVal << '\n';

    std::vector<uint8_t> bytes(grid.size());
    for (size_t i=0; i<dims.x*size_t(dims.y)*dims.z; ++i) {
        if (std::isnan(grid[i])) bytes[i] = 0;
        else {
            float val01 = (grid[i] - minVal) / (maxVal - minVal);
            uint8_t valb(val01*255.999f);
            bytes[i] = valb;
        }
    }

    std::ofstream outFile("temp.raw");
    outFile.write((const char *)bytes.data(),bytes.size());
}


