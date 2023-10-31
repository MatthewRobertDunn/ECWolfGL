#include "WallGenerator.h"

namespace MatGl {
	using namespace glm;
	VertexList CreateSouthWall(vec2 pos, vec4 color, float layer) {
		vec3 pos3 = vec3(pos, 0);
		VertexList wall{
			//Triangle 1
			Vertex{
					vec3(0.0f, 0.0f , FLOOR_HEIGHT + WALL_HEIGHT) + pos3,  //TL
					color,
					vec3(0.0,1.0,layer),
				},
			Vertex{
					vec3(0.0f, 0.0f , FLOOR_HEIGHT) + pos3,		//BL
					color,
					vec3(0.0,0.0,layer),
				},
			Vertex{
					vec3(WALL_WIDTH, 0.0f , FLOOR_HEIGHT + WALL_HEIGHT) + pos3, //TR
					color,
					vec3(1.0,1.0,layer),
				},
				//Triangle 2
				Vertex{
						vec3(WALL_WIDTH, 0.0f , FLOOR_HEIGHT + WALL_HEIGHT) + pos3, //TR
						color,
						vec3(1.0,1.0,layer),
					},
				Vertex{
						vec3(0.0f, 0.0f , FLOOR_HEIGHT) + pos3,			//BL
						color,
						vec3(0.0,0.0,layer),
					},
				Vertex{
						vec3(WALL_WIDTH, 0.0f , FLOOR_HEIGHT) + pos3, //BR
						color,
						vec3(1.0,0.0,layer),
					}
		};
		return wall;
	}



	VertexList CreateNorthWall(vec2 pos, vec4 color, float layer) {
		vec3 pos3 = vec3(pos, 0);
		VertexList wall{
			//Triangle 1
			Vertex{
					vec3(0.0f, 0.0f , FLOOR_HEIGHT + WALL_HEIGHT) + pos3,  //TL
					color,
					vec3(1.0,1.0,layer),
				},
			Vertex{
					vec3(0.0f, 0.0f , FLOOR_HEIGHT) + pos3,		//BL
					color,
					vec3(1.0,0.0,layer),
				},
			Vertex{
					vec3(WALL_WIDTH, 0.0f , FLOOR_HEIGHT + WALL_HEIGHT) + pos3, //TR
					color,
					vec3(0.0,1.0,layer),
				},
			//Triangle 2
			Vertex{
					vec3(WALL_WIDTH, 0.0f , FLOOR_HEIGHT + WALL_HEIGHT) + pos3, //TR
					color,
					vec3(0.0,1.0,layer),
				},
			Vertex{
					vec3(0.0f, 0.0f , FLOOR_HEIGHT) + pos3,			//BL
					color,
					vec3(1.0,0.0,layer),
				},
			Vertex{
					vec3(WALL_WIDTH, 0.0f , FLOOR_HEIGHT) + pos3, //BR
					color,
					vec3(0.0,0.0,layer),
					}
		};
		return wall;
	}
	VertexList CreateEastWall(glm::vec2 pos, glm::vec4 color, float layer)
	{
		vec3 pos3 = vec3(pos, 0);
		VertexList wall{
			//Triangle 1
			Vertex{
					vec3(0.0f, 0.0f , FLOOR_HEIGHT + WALL_HEIGHT) + pos3,  //TL
					color,
					vec3(0.0,1.0,layer),
				},
			Vertex{
					vec3(0.0f, 0.0f , FLOOR_HEIGHT) + pos3,		//BL
					color,
					vec3(0.0,0.0,layer),
				},
			Vertex{
					vec3(0.0f,WALL_WIDTH , FLOOR_HEIGHT + WALL_HEIGHT) + pos3, //TR
					color,
					vec3(1.0,1.0,layer),
				},
				//Triangle 2
			Vertex{
					vec3(0.0f, WALL_WIDTH , FLOOR_HEIGHT + WALL_HEIGHT) + pos3, //TR
					color,
					vec3(1.0,1.0,layer),
				},
			Vertex{
					vec3(0.0f, 0.0f , FLOOR_HEIGHT) + pos3,			//BL
					color,
					vec3(0.0,0.0,layer),
				},
			Vertex{
					vec3(0.0f, WALL_WIDTH , FLOOR_HEIGHT) + pos3, //BR
					color,
					vec3(1.0,0.0,layer),
				}
		};
		return wall;
	}
	VertexList CreateWestWall(glm::vec2 pos, glm::vec4 color, float layer)
	{
		vec3 pos3 = vec3(pos, 0);
		VertexList wall{
			//Triangle 1
			Vertex{
					vec3(0.0f, 0.0f , FLOOR_HEIGHT + WALL_HEIGHT) + pos3,  //TL
					color,
					vec3(1.0,1.0,layer),
				},
			Vertex{
					vec3(0.0f, 0.0f , FLOOR_HEIGHT) + pos3,		//BL
					color,
					vec3(1.0,0.0,layer),
				},
			Vertex{
					vec3(0.0f,WALL_WIDTH , FLOOR_HEIGHT + WALL_HEIGHT) + pos3, //TR
					color,
					vec3(0.0,1.0,layer),
				},
				//Triangle 2
			Vertex{
					vec3(0.0f, WALL_WIDTH , FLOOR_HEIGHT + WALL_HEIGHT) + pos3, //TR
					color,
					vec3(0.0,1.0,layer),
				},
			Vertex{
					vec3(0.0f, 0.0f , FLOOR_HEIGHT) + pos3,			//BL
					color,
					vec3(1.0,0.0,layer),
				},
			Vertex{
					vec3(0.0f, WALL_WIDTH , FLOOR_HEIGHT) + pos3, //BR
					color,
					vec3(0.0,0.0,layer),
				}
		};
		return wall;
	}
}