struct ray 
{
  Vec2<float> position;
  Vec2<float> velocity;
  float intensity;
  png::rgb_pixel color;
};

typedef struct ray lightray;

inline bool loadFromFile(std::string fileName,
                         std::vector<lightray> &lightSources) {
  std::ifstream f(fileName);
  assert((bool)f && "Cannot open input file");

  std::string line;
  while (std::getline(f, line)) {
    lightray pRay;
    std::stringstream sstream(line);
    std::string str;
    std::getline(sstream, str, ' ');
    pRay.position.x = (float)atof(str.c_str());
    std::getline(sstream, str, ' ');
    pRay.position.y = (float)atof(str.c_str());
    std::getline(sstream, str, ' ');
    pRay.color.red = (int)atoi(str.c_str());
    std::getline(sstream, str, ' ');
    pRay.color.green = (int)atoi(str.c_str());
    std::getline(sstream, str, ' ');
    pRay.color.blue = (int)atoi(str.c_str());
    std::getline(sstream, str, '\n');
    pRay.intensity = (float)atof(str.c_str());
    lightSources.push_back(pRay);
  }
  return true;
}