// #####################################################################
//  Main
//  CS3451 Computer Graphics Starter Code
//  Contact: Bo Zhu (bo.zhu@gatech.edu)
// #####################################################################
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <unordered_set>

#include "OpenGLMesh.h"
#include "OpenGLMarkerObjects.h"
#include "OpenGLCommon.h"
#include "OpenGLWindow.h"
#include "OpenGLViewer.h"
#include "TinyObjLoader.h"

#ifndef __Main_cpp__
#define __Main_cpp__

#ifdef __APPLE__
#define CLOCKS_PER_SEC 100000
#endif

#define DegreesToRadians(degrees) (degrees * M_PI / 180)

class MyDriver : public OpenGLViewer
{
	std::vector<OpenGLTriangleMesh *> mesh_object_array; ////mesh objects, every object you put in this array will be rendered.
	OpenGLSegmentMesh *floor = nullptr;
	std::vector<OpenGLSegmentMesh *> coords, trajectory;
	clock_t startTime;

public:
	virtual void Initialize()
	{
		draw_axes = false;
		startTime = clock();
		OpenGLViewer::Initialize();
		opengl_window->camera_distance = 25.f;
		opengl_window->camera_target = Vector3f(0, 3.5, 0);
		opengl_window->Update_Clip_Planes();
	}

	void Create_Background(const OpenGLColor &color1, const OpenGLColor &color2)
	{
		auto bg = Add_Interactive_Object<OpenGLBackground>();
		bg->Set_Color(color1, color2);
		bg->Initialize();
	}

	virtual void Initialize_Data()
	{
		Create_Background(OpenGLColor(0.71f, 0.6f, 0.17f, 1.f), OpenGLColor(0.71f, 0.87f, 0.17f, 1.f));
		OpenGLShaderLibrary::Instance()->Add_Shader_From_File("a3_vert.vert", "a3_frag.frag", "a3_shading"); ////bind shader for this assignment

		//// Create_Angry_Bird_Palace(); //// TODO: Comment this line when you start to implement your customized scene
		Create_Angry_Bird_Garden(); //// TODO: Uncomment this line when you start to implement your customized scene
	}

	void Create_Angry_Bird_Palace()
	{
		//// draw the three axes
		Add_Coord({Vector3(0, 0.01, 0), Vector3(5, 0.01, 0)}, OpenGLColor(1, 0, 0, 1)); //// X axis
		Add_Coord({Vector3(0, 0, 0), Vector3(0, 5, 0)}, OpenGLColor(0, 1, 0, 1));		//// Y axis
		Add_Coord({Vector3(0, 0.01, 0), Vector3(0, 0.01, 5)}, OpenGLColor(0, 0, 1, 1)); //// Z zxis

		//// draw the ground
		Add_Ground();

		//// Step 1: add the castle by reading the model from "castle.obj"
		//// The model needs to undergo the following transform operations in sequence:
		//// (1) rotate *counterclockwisely* around the y-axis by 90 degrees,
		//// (2) uniformly scale by a factor of 5,
		//// (3) translate upwards by 1.3 units in the y direction.
		//// Your task is to specify the values of the 4x4 transform matrix and send it to the mesh model via Set_Model_Matrix().
		//// You are allowed to use a chain of matrix multiplications to calculate the matrix.

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		auto castle = Add_Obj_Mesh_Object_From_File("castle.obj", OpenGLColor(.6f, .6f, .6f, 1.f));
		{
			Matrix4f r;
			r << cos(DegreesToRadians(90)), 0., sin(DegreesToRadians(90)), 0.,
				0., 1., 0., 0.,
				-sin(DegreesToRadians(90)), 0., cos(DegreesToRadians(90)), 0.,
				0., 0., 0., 1.;

			Matrix4f s;
			s << 5., 0., 0., 0.,
				0., 5., 0., 0.,
				0., 0., 5., 0.,
				0., 0., 0., 1.;

			Matrix4f t;
			t << 1., 0., 0., 0.,
				0., 1., 0., 1.3,
				0., 0., 1., 0.,
				0., 0., 0., 1.;

			castle->Set_Model_Matrix(t * s * r);
		}
		/* Your implementation ends. */

		//// Step 2: add the axes statue by reading the model from "axes.obj"
		//// The model needs to undergo the following transform operations in sequence:
		//// (1) rotate *counterclockwisely* around the y-axis by 90 degrees,
		//// (2) uniformly scale by a factor of 2,
		//// (3) translate by 6 units in the positive x direction and 1 unit in the y direction.

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		auto axes = Add_Obj_Mesh_Object_From_File("axes.obj", OpenGLColor(.9f, .5f, .0f, 1.f));
		{
			Matrix4f r;
			r << cos(DegreesToRadians(90)), 0., sin(DegreesToRadians(90)), 0.,
				0., 1., 0., 0.,
				-sin(DegreesToRadians(90)), 0., cos(DegreesToRadians(90)), 0.,
				0., 0., 0., 1.;

			Matrix4f s;
			s << 2., 0., 0., 0.,
				0., 2., 0., 0.,
				0., 0., 2., 0.,
				0., 0., 0., 1.;

			Matrix4f t;
			t << 1., 0., 0., 6.,
				0., 1., 0., 1.,
				0., 0., 1., 0.,
				0., 0., 0., 1.;

			axes->Set_Model_Matrix(t * s * r);
		}
		/* Your implementation ends. */

		//// Step 3: add the magic tower by reading the model from "tower.obj"
		//// The model needs to undergo the following transform operations in sequence:
		//// (1) rotate *clockwisely* around the y-axis by 45 degrees,
		//// (2) non-uniformly scale by factors of 2, 6, 2 in the x, y, z directions
		//// (3) translate by 6 units in the negative x direction and 3 units in the y direction.

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		auto tower = Add_Obj_Mesh_Object_From_File("tower.obj", OpenGLColor(.0f, .5f, .5f, 1.f));
		{
			Matrix4f r;
			r << cos(DegreesToRadians(315)), 0., sin(DegreesToRadians(315)), 0.,
				0., 1., 0., 0.,
				-sin(DegreesToRadians(315)), 0., cos(DegreesToRadians(315)), 0.,
				0., 0., 0., 1.;

			Matrix4f s;
			s << 2., 0., 0., 0.,
				0., 6., 0., 0.,
				0., 0., 2., 0.,
				0., 0., 0., 1.;

			Matrix4f t;
			t << 1., 0., 0., -6.,
				0., 1., 0., 3.,
				0., 0., 1., 0.,
				0., 0., 0., 1.;

			tower->Set_Model_Matrix(t * s * r);
		}
		/* Your implementation ends. */

		//// Step 4: add 24 trees by reading the model from "tree1.obj"
		//// The 24 trees need to be distributed evenly and at equal distances along the circumference of the inner circle.
		//// The circle has its center at the origin and a radius of 8.
		//// Each tree needs to be translated in the positive y axis by 0.5 unit to ensure its base is above the ground.
		//// Calculate the transform matrix for each tree in the following for-loop.

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		int tree_num = 24;
		for (int i = 0; i < tree_num; i++)
		{
			auto tree = Add_Obj_Mesh_Object_From_File("tree1.obj", OpenGLColor(0.f, 1.f, 0.f, 1.f));
			{
				double angle = DegreesToRadians(i * (360. / tree_num));
				double x = 8. * cos(angle);
				double z = 8. * sin(angle);

				Matrix4f t;
				t << 1., 0., 0., x,
					0., 1., 0., 0.5,
					0., 0., 1., z,
					0., 0., 0., 1.;
				tree->Set_Model_Matrix(t);
			}
		}
		/* Your implementation ends. */

		//// Step 5: add 36 trees by reading the model from "tree2.obj"
		//// The 36 trees need to be distributed evenly and at equal distances along the circumference of the outer circle.
		//// The circle has its center at the origin and a radius of 10.
		//// Each tree needs to be translated in the positive y axis by 0.5 unit to ensure its base is above the ground.
		//// Calculate the transform matrix for each tree in the following for-loop.

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		int tree2_num = 36;
		for (int i = 0; i < tree2_num; i++)
		{
			auto tree = Add_Obj_Mesh_Object_From_File("tree2.obj", OpenGLColor(0.f, 1.f, 0.f, 1.f));
			{
				double angle = DegreesToRadians(i * (360. / tree2_num));
				double x = 10. * cos(angle);
				double z = 10. * sin(angle);

				Matrix4f t;
				t << 1., 0., 0., x,
					0., 1., 0., 0.5,
					0., 0., 1., z,
					0., 0., 0., 1.;
				tree->Set_Model_Matrix(t);
			}
		}
		/* Your implementation ends. */

		//// Step 6: add 5 stone steps by using the function `Add_Cube()`
		//// `Add_Cube()` adds a cube mesh to the scene with its center at the origin and each side measuring 1 unit.
		//// Each stone step has its size as 1, 0.1, and 0.5 along x, y, z axes.
		//// The center of the first stone step is in (0, 0, 3), incremented by 1 in positive z direction for the following stone steps.
		//// Calculate the transform matrix for each stone step in the following for-loop.

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		for (int i = 0; i < 5; i++)
		{
			auto cube1 = Add_Cube(1.f, OpenGLColor(0.1f, 0.1f, 0.1f, 1.f));
			{
				Matrix4f t;
				t << 1., 0., 0., 0.,
					0., .1, 0., 0.,
					0., 0., .5, (double)(3 + i),
					0., 0., 0., 1.;
				cube1->Set_Model_Matrix(t);
			}
		}
		/* Your implementation ends. */

		//// Step 7: add 6 keyframes of a throwing angry bird following a parabola.
		//// The angry bird is thrown from (-5, 0, 0) with initial velocity (5, 9.8, 0) (unit/sec)
		//// Its angular velocity omega = 150 (deg/sec) *clockwise*
		//// The gravity is g = 9.8 (unit/sec) in negative y direction
		//// The x coordinate of the bird can be calculated as x = x0 + ux * t
		//// The y coordinate of the bird can be calculated as y = uy * t - 0.5 * g * t * t
		//// Your task is to draw keyframes at time t = 0.2, 0.5, 0.8, 1.1, 1.4, 1.7 respectively.
		//// To this end, you need to calculate the transform matrix for each keyframe of the angry bird in the following for-loop.
		//// You can temporarily uncomment the following line to visualize the parabola trajectory as a reference during implementation.
		//// Comment it out again once you complete the task.

		//// Add_Arc_Trajectory();

		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */
		std::vector<float> time = {0.2, 0.5, 0.8, 1.1, 1.4, 1.7};
		int bird_num = 6;
		for (int i = 0; i < bird_num; i++)
		{
			auto bird = Add_Obj_Mesh_Object_From_File("bird.obj", OpenGLColor(1.f, 0.2f, 0.f, 1.f));
			{
				double x = -5. + (5. * time[i]);
				double y = (9.8 * time[i]) - (0.5 * (9.8 * (time[i] * time[i])));
				double z = 0.;
				double orientation = DegreesToRadians(-150. * time[i]);

				Matrix4f t;
				t << cos(orientation), -sin(orientation), 0., x,
					sin(orientation), cos(orientation), 0., y,
					0., 0., 1., z,
					0., 0., 0., 1.;

				bird->Set_Model_Matrix(t);
			}
		}
		/* Your implementation ends. */
	}

	//// Step 8: Create a new garden scene by using the mesh objects we provided, or download your own from online resources.
	//// Practise matrix transformation by mimicking the way we setup matrices in the `Create_Angry_Bird_Palace()` function.

	void Create_Angry_Bird_Garden()
	{
		/* Your implementation starts. You may add/remove/edit any part of the code in the following. */

		//// draw the ground
		Add_Ground();

		//// helper methods for transforms ////
		// translate
		auto T = [](float x, float y, float z)
		{
			Matrix4f m;
			m << 1.f, 0.f, 0.f, x,
				0.f, 1.f, 0.f, y,
				0.f, 0.f, 1.f, z,
				0.f, 0.f, 0.f, 1.f;
			return m;
		};

		// scale
		auto S = [](float x, float y, float z)
		{
			Matrix4f m;
			m << x, 0.f, 0.f, 0.f,
				0.f, y, 0.f, 0.f,
				0.f, 0.f, z, 0.f,
				0.f, 0.f, 0.f, 1.f;
			return m;
		};

		// rotate around Y-axis
		auto RY = [](float degrees)
		{
			float rad = DegreesToRadians(degrees);
			Matrix4f m;
			m << cos(rad), 0.f, sin(rad), 0.f,
				0.f, 1.f, 0.f, 0.f,
				-sin(rad), 0.f, cos(rad), 0.f,
				0.f, 0.f, 0.f, 1.f;
			return m;
		};

		const std::string garden_path = "garden/garden/";

		// green ground
		auto green_ground = Add_Obj_Mesh_Object_From_File(garden_path + "ground.obj", OpenGLColor(0.45f, 0.78f, 0.35f, 1.f));
		green_ground->Set_Model_Matrix(T(0.f, -0.02f, 0.f) * S(20.f, 1.f, 23.f));

		// fountain
		auto fountain = Add_Obj_Mesh_Object_From_File(garden_path + "fountain1.obj", OpenGLColor(1.f, 1.f, 1.f, 1.f));
		fountain->Set_Model_Matrix(T(0.f, 1.5f, 0.f) * S(3.0f, 3.0f, 3.0f));

		// pavilion
		auto pavilion = Add_Obj_Mesh_Object_From_File(garden_path + "building1.obj", OpenGLColor(0.78f, 0.56f, 0.33f, 1.f));
		pavilion->Set_Model_Matrix(T(0.2f, 0.45f, -6.8f) * RY(90.f) * S(2.4f, 2.4f, 2.4f));

		// stone walkways
		for (int i = 0; i < 6; i++)
		{
			// direction
			float d = 2.2f + i * 0.9f;

			// cool grey color
			OpenGLColor stone_color = (i % 2 == 0) ? OpenGLColor(0.56f, 0.54f, 0.49f, 1.f) : OpenGLColor(0.44f, 0.43f, 0.40f, 1.f);

			auto north = Add_Cube(1.f, stone_color);
			north->Set_Model_Matrix(T(0.f, 0.03f, -d) * S(0.95f, 0.08f, 0.55f));
			auto south = Add_Cube(1.f, stone_color);
			south->Set_Model_Matrix(T(0.f, 0.03f, d) * S(0.95f, 0.08f, 0.55f));
			auto east = Add_Cube(1.f, stone_color);
			east->Set_Model_Matrix(T(d, 0.03f, 0.f) * S(0.55f, 0.08f, 0.95f));
			auto west = Add_Cube(1.f, stone_color);
			west->Set_Model_Matrix(T(-d, 0.03f, 0.f) * S(0.55f, 0.08f, 0.95f));
		}

		// trees
		int tree_count = 32;
		float tree_radius = 8.8f;
		for (int i = 0; i < tree_count; i++)
		{
			float angle = i * 360.f / (float)tree_count;
			float a = DegreesToRadians(angle);
			float x = tree_radius * cos(a);
			float z = tree_radius * sin(a);

			// alernate tree type and color
			std::string tree_file = (i % 2 == 0) ? "garden_tree1.obj" : "garden_tree2.obj";
			OpenGLColor tree_color = (i % 2 == 0) ? OpenGLColor(0.10f, 0.45f, 0.18f, 1.f) : OpenGLColor(0.20f, 0.58f, 0.25f, 1.f);

			auto tree = Add_Obj_Mesh_Object_From_File(garden_path + tree_file, tree_color);
			tree->Set_Model_Matrix(T(x, 0.75f, z) * RY(-angle + 90.f) * S(1.75f, 1.75f, 1.75f));
		}

		// lanterns
		for (int i = 0; i < 5; i++)
		{
			float z = 2.35f + i * 1.15f;
			for (int side = -1; side <= 1; side += 2)
			{
				auto light = Add_Obj_Mesh_Object_From_File(garden_path + "light1.obj", OpenGLColor(1.00f, 0.78f, 0.28f, 1.f));
				light->Set_Model_Matrix(T(side * 1.65f, 0.35f, z) * RY(side < 0 ? 18.f : -18.f) * S(0.75f, 0.75f, 0.75f));
			}
		}

		// table and chairs
		auto table = Add_Obj_Mesh_Object_From_File(garden_path + "table1.obj", OpenGLColor(0.47f, 0.27f, 0.13f, 1.f));
		table->Set_Model_Matrix(T(-5.2f, 0.32f, -2.8f) * RY(25.f) * S(1.25f, 1.25f, 1.25f));

		// define chair types, postitions, and rotations
		std::vector<std::string> chairs = {"chair1.obj", "chair2.obj", "chair3.obj", "chair4.obj"};
		std::vector<Vector3> chair_pos = {Vector3(-6.4, 0.34, -2.8), Vector3(-4.0, 0.34, -2.8), Vector3(-5.2, 0.34, -3.95), Vector3(-5.2, 0.34, -1.65)};
		std::vector<float> chair_rot = {90.f, 270.f, 0.f, 180.f};

		for (int i = 0; i < 4; i++)
		{
			auto chair = Add_Obj_Mesh_Object_From_File(garden_path + chairs[i], OpenGLColor(0.60f, 0.36f, 0.18f, 1.f));
			chair->Set_Model_Matrix(T(chair_pos[i][0], chair_pos[i][1], chair_pos[i][2]) * RY(chair_rot[i] + 25.f) * S(0.95f, 0.95f, 0.95f));
		}

		auto table2 = Add_Obj_Mesh_Object_From_File(garden_path + "table1.obj", OpenGLColor(0.47f, 0.27f, 0.13f, 1.f));
		table2->Set_Model_Matrix(T(5.0f, 0.32f, 3.2f) * RY(-25.f) * S(1.25f, 1.25f, 1.25f));

		// add another table/chair set in opposite quadrant
		std::vector<Vector3> chair2_pos = {
			Vector3(3.8, 0.34, 3.2),
			Vector3(6.2, 0.34, 3.2),
			Vector3(5.0, 0.34, 2.05),
			Vector3(5.0, 0.34, 4.35)};
		std::vector<float> chair2_rot = {270.f, 90.f, 180.f, 0.f};

		for (int i = 0; i < 4; i++)
		{
			auto chair = Add_Obj_Mesh_Object_From_File(garden_path + chairs[i], OpenGLColor(0.60f, 0.36f, 0.18f, 1.f));
			chair->Set_Model_Matrix(
				T(chair2_pos[i][0], chair2_pos[i][1], chair2_pos[i][2]) * RY(chair2_rot[i] - 25.f) * S(0.95f, 0.95f, 0.95f));
		}

		// add birds on chairs
		std::vector<int> bird_chairs = {0, 2};
		std::vector<OpenGLColor> bird_colors = {
			OpenGLColor(0.90f, 0.08f, 0.05f, 1.f),
			OpenGLColor(0.95f, 0.82f, 0.12f, 1.f)};
		for (int i = 0; i < bird_chairs.size(); i++)
		{
			int chair_idx = bird_chairs[i];
			auto bird = Add_Obj_Mesh_Object_From_File(garden_path + "bird.obj", bird_colors[i]);
			bird->Set_Model_Matrix(
				T(chair_pos[chair_idx][0], 0.55f, chair_pos[chair_idx][2]) * RY(chair_rot[chair_idx] + 25.f) * S(0.42f, 0.42f, 0.42f));
		}

		std::vector<int> bird2_chairs = {1, 3};
		std::vector<OpenGLColor> bird2_colors = {
			OpenGLColor(0.08f, 0.28f, 0.85f, 1.f),
			OpenGLColor(0.98f, 0.45f, 0.10f, 1.f)};
		for (int i = 0; i < bird2_chairs.size(); i++)
		{
			int chair_idx = bird2_chairs[i];
			auto bird = Add_Obj_Mesh_Object_From_File(garden_path + "bird.obj", bird2_colors[i]);
			bird->Set_Model_Matrix(
				T(chair2_pos[chair_idx][0], 0.65f, chair2_pos[chair_idx][2]) * RY(chair2_rot[chair_idx] - 25.f) * S(0.42f, 0.42f, 0.42f));
		}

		// vases
		auto vase1 = Add_Obj_Mesh_Object_From_File(garden_path + "vase1.obj", OpenGLColor(0.70f, 0.24f, 0.19f, 1.f));
		vase1->Set_Model_Matrix(T(1.0f, 0.5f, 1.0f));
		auto vase2 = Add_Obj_Mesh_Object_From_File(garden_path + "vase1.obj", OpenGLColor(0.70f, 0.24f, 0.19f, 1.f));
		vase2->Set_Model_Matrix(T(-1.0f, 0.5f, -1.0f));
		auto vase3 = Add_Obj_Mesh_Object_From_File(garden_path + "vase1.obj", OpenGLColor(0.70f, 0.24f, 0.19f, 1.f));
		vase3->Set_Model_Matrix(T(1.0f, 0.5f, -1.0f));
		auto vase4 = Add_Obj_Mesh_Object_From_File(garden_path + "vase1.obj", OpenGLColor(0.70f, 0.24f, 0.19f, 1.f));
		vase4->Set_Model_Matrix(T(-1.0f, 0.5f, 1.0f));

		// grass
		auto grass1 = Add_Obj_Mesh_Object_From_File(garden_path + "grass2.obj", OpenGLColor(0.10f, 0.45f, 0.18f, 1.f));
		grass1->Set_Model_Matrix(T(4.f, 0.75f, -4.f) * S(3.f, 3.f, 3.f));
		auto grass2 = Add_Obj_Mesh_Object_From_File(garden_path + "grass2.obj", OpenGLColor(0.10f, 0.45f, 0.18f, 1.f));
		grass2->Set_Model_Matrix(T(4.f, 0.75f, -3.4f) * RY(180) * S(3.f, 3.f, 3.f));
		auto grass3 = Add_Obj_Mesh_Object_From_File(garden_path + "grass2.obj", OpenGLColor(0.10f, 0.45f, 0.18f, 1.f));
		grass3->Set_Model_Matrix(T(4.3f, 0.75f, -3.7f) * RY(-90) * S(3.f, 3.f, 3.f));
		auto grass4 = Add_Obj_Mesh_Object_From_File(garden_path + "grass2.obj", OpenGLColor(0.10f, 0.45f, 0.18f, 1.f));
		grass4->Set_Model_Matrix(T(3.8f, 0.75f, -3.7f) * RY(90) * S(3.f, 3.f, 3.f));

		// lights
		auto light1 = Add_Obj_Mesh_Object_From_File(garden_path + "light2.obj", OpenGLColor(0.56f, 0.54f, 0.49f, 1.f));
		light1->Set_Model_Matrix(T(-5.f, 0.75f, 4.f) * S(1.5f, 1.5f, 1.5f));
		auto light2 = Add_Obj_Mesh_Object_From_File(garden_path + "light2.obj", OpenGLColor(0.56f, 0.54f, 0.49f, 1.f));
		light2->Set_Model_Matrix(T(-3.f, 0.75f, 4.f) * S(1.5f, 1.5f, 1.5f));

		// birds around the lights
		auto bird1 = Add_Obj_Mesh_Object_From_File(garden_path + "bird.obj", OpenGLColor(0.08f, 0.28f, 0.85f, 1.f));
		bird1->Set_Model_Matrix(T(-5.f, 1.7f, 4.f) * RY(120) * S(0.42f, 0.42f, 0.42f));
		auto bird2 = Add_Obj_Mesh_Object_From_File(garden_path + "bird.obj", OpenGLColor(0.95f, 0.82f, 0.12f, 1.f));
		bird2->Set_Model_Matrix(T(-4.f, .25f, 3.5f) * RY(-50) * S(0.42f, 0.42f, 0.42f));

		// fence
		for (int i = 0; i < 16; i++)
		{
			float offset = -9.f + i * 1.2f;

			auto north_fence = Add_Obj_Mesh_Object_From_File(garden_path + "fence1.obj", OpenGLColor(1.0f, 0.72f, 0.18f, 1.f));
			north_fence->Set_Model_Matrix(T(offset, 0.25f, -10.f) * S(2.f, 1.75f, 1.35f));

			auto south_fence = Add_Obj_Mesh_Object_From_File(garden_path + "fence1.obj", OpenGLColor(1.0f, 0.72f, 0.18f, 1.f));
			south_fence->Set_Model_Matrix(T(offset, 0.25f, 10.f) * RY(180.f) * S(2.f, 1.75f, 1.35f));

			auto east_fence = Add_Obj_Mesh_Object_From_File(garden_path + "fence1.obj", OpenGLColor(1.0f, 0.72f, 0.18f, 1.f));
			east_fence->Set_Model_Matrix(T(10.f, 0.25f, offset) * RY(90.f) * S(2.f, 1.75f, 1.35f));

			auto west_fence = Add_Obj_Mesh_Object_From_File(garden_path + "fence1.obj", OpenGLColor(1.0f, 0.72f, 0.18f, 1.f));
			west_fence->Set_Model_Matrix(T(-10.f, 0.25f, offset) * RY(270.f) * S(2.f, 1.75f, 1.35f));
		}
		/* Your implementation ends. */
	}

	//////////////////////////////////////////////////////////////
	//// The following functions are auxiliary functions to add mesh objects into the scene
	//////////////////////////////////////////////////////////////

	//// This function adds a mesh object from an obj file
	int Add_Obj_Mesh_Object(std::string obj_file_name)
	{
		auto mesh_obj = Add_Interactive_Object<OpenGLTriangleMesh>();

		Array<std::shared_ptr<TriangleMesh<3>>> meshes;
		Obj::Read_From_Obj_File(obj_file_name, meshes);
		mesh_obj->mesh = *meshes[0];
		std::cout << "load tri_mesh from obj file, #vtx: " << mesh_obj->mesh.Vertices().size() << ", #ele: " << mesh_obj->mesh.Elements().size() << std::endl;

		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size() - 1;
	}

	//// This function adds a mesh object from an .obj file and allows the user to specify its model matrix
	OpenGLTriangleMesh *Add_Obj_Mesh_Object_From_File(std::string file_name, OpenGLColor color = OpenGLColor::White())
	{
		int obj_idx = Add_Obj_Mesh_Object(file_name);
		auto obj = mesh_object_array[obj_idx];
		obj->color = color; // set color

		obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("a3_shading"));
		Set_Polygon_Mode(obj, PolygonMode::Fill);
		Set_Shading_Mode(obj, ShadingMode::A2);
		obj->Set_Data_Refreshed();
		obj->Initialize();

		return obj;
	}
	//// This function adds a cube with a specified length and color
	OpenGLTriangleMesh *Add_Cube(float length = 1, OpenGLColor color = OpenGLColor::White())
	{
		auto obj = Add_Interactive_Object<OpenGLTriangleMesh>();
		mesh_object_array.push_back(obj);
		obj->color = color; // set color
		// set up vertices and elements
		std::vector<Vector3> vertices{Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(1, 1, 0), Vector3(0, 0, 1), Vector3(1, 0, 1), Vector3(0, 1, 1), Vector3(1, 1, 1)};
		std::vector<Vector3i> elements{Vector3i(4, 5, 7), Vector3i(4, 7, 6),
									   Vector3i(5, 1, 7), Vector3i(7, 1, 3),
									   Vector3i(2, 3, 1), Vector3i(0, 2, 1),
									   Vector3i(6, 2, 4), Vector3i(2, 0, 4),
									   Vector3i(2, 6, 3), Vector3i(6, 7, 3),
									   Vector3i(0, 1, 4), Vector3i(1, 5, 4)};
		for (auto &v3 : vertices)
		{
			v3 -= Vector3(0.5, 0.5, 0.5);
			v3 *= length;
		};
		obj->mesh.Vertices() = vertices;
		obj->mesh.Elements() = elements;

		obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("a3_shading"));
		Set_Polygon_Mode(obj, PolygonMode::Fill);
		Set_Shading_Mode(obj, ShadingMode::A2);
		obj->Set_Data_Refreshed();
		obj->Initialize();

		return obj;
	}

	//// This function adds an axis with a specified color
	void Add_Coord(std::vector<Vector3> pts, OpenGLColor color)
	{
		auto axis = Add_Interactive_Object<OpenGLSegmentMesh>();
		coords.push_back(axis);
		Set_Polygon_Mode(axis, PolygonMode::Fill);
		Set_Shading_Mode(axis, ShadingMode::None);
		auto line_mesh = &axis->mesh;
		line_mesh->Vertices() = pts;
		line_mesh->Elements() = {Vector2i(0, 1)};
		Set_Line_Width(axis, 2.5f);
		Set_Color(axis, color);
		axis->Set_Data_Refreshed();
		axis->Initialize();
	}

	//// This function adds a wireframe ground
	void Add_Ground()
	{
		floor = Add_Interactive_Object<OpenGLSegmentMesh>();
		Set_Polygon_Mode(floor, PolygonMode::Fill);
		Set_Shading_Mode(floor, ShadingMode::None);
		auto line_mesh = &floor->mesh;
		std::vector<Vector3> pts;
		std::vector<Vector2i> eles;
		for (int i = -10; i <= 10; i++)
		{
			pts.push_back(Vector3(i, 0, -10));
			pts.push_back(Vector3(i, 0, 10));
			pts.push_back(Vector3(-10, 0, i));
			pts.push_back(Vector3(10, 0, i));
		}
		for (int i = 0; i < pts.size(); i += 2)
		{
			eles.push_back(Vector2i(i, i + 1));
		}
		line_mesh->Vertices() = pts;
		line_mesh->Elements() = eles;
		Set_Line_Width(floor, 3.f);
		Set_Color(floor, OpenGLColor(0, 0, 0, 1));
		floor->Set_Data_Refreshed();
		floor->Initialize();
	}

	//// This function adds a parabola trajectory
	void Add_Arc_Trajectory()
	{
		float ux = 5, uy = 9.8, g = 9.8;
		std::vector<Vector3> pts;
		std::vector<Vector2i> eles;
		for (float t = 0; t <= 2; t += 0.02)
		{
			float x = ux * t - 5;
			float y = uy * t - 0.5 * g * t * t;
			pts.push_back(Vector3(x, y, 0));
		}
		for (int i = 0; i < pts.size() - 1; i++)
		{
			eles.push_back(Vector2i(i, i + 1));
		}
		auto traj = Add_Interactive_Object<OpenGLSegmentMesh>();
		trajectory.push_back(traj);
		Set_Polygon_Mode(traj, PolygonMode::Fill);
		Set_Shading_Mode(traj, ShadingMode::None);
		auto line_mesh = &traj->mesh;
		line_mesh->Vertices() = pts;
		line_mesh->Elements() = eles;
		Set_Line_Width(traj, 2.f);
		Set_Color(traj, OpenGLColor(0, 0, 1, 1));
		traj->Set_Data_Refreshed();
		traj->Initialize();
	}

	void Init_Bird_Vertices(OpenGLTriangleMesh *obj)
	{
		std::vector<Vector3> &vertices = obj->mesh.Vertices();
		for (auto &v : vertices)
		{
			v += Vector3(-8.35, 0, 0);
			auto x = v.x();
			v.x() = v.z();
			v.z() = -x;
			v /= 10.f;
		}
	}

	//// Go to next frame
	virtual void Toggle_Next_Frame()
	{
		for (auto &mesh_obj : mesh_object_array)
		{
			mesh_obj->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
		}
		OpenGLViewer::Toggle_Next_Frame();
	}

	virtual void Run()
	{
		OpenGLViewer::Run();
	}
};

int main(int argc, char *argv[])
{
	MyDriver driver;
	driver.Initialize();
	driver.Run();
}

#endif
