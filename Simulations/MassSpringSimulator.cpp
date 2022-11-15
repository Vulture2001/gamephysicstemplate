#include "MassSpringSystemSimulator.h"
#include "MassPoint.h"
#include "Spring.h"

MassSpringSystemSimulator::MassSpringSystemSimulator()
{
	m_fMass = 10;
	m_fStiffness = 40;
	m_fDamping = 0;
	m_iIntegrator = 0;
	h = 0.005;
	currentCalculationMode = 0;
	gravity = Vec3 (0, -1, 0);
	isThereGravity = false;
	invertedVelocity = false;

}


const char* MassSpringSystemSimulator::getTestCasesStr()
{
	return "BasicTest, Euler, Midpoint, Simulation";
}

// Pls select your Simulation via the Default function
void MassSpringSystemSimulator::reset()
{
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
	masspoints.clear();
	springs.clear();
	
	switch (m_iIntegrator)
	{
	case 0:
		masspoints.push_back(MassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), Vec3(0, 0, 0), false, Vec3(0, 0, 0)));
		masspoints.push_back(MassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), Vec3(0, 0, 0), false, Vec3(0, 0, 0)));
		springs.push_back(Spring(0, 1, 1));
		break;
	case 1:
		masspoints.push_back(MassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), Vec3(0, 0, 0), false, Vec3(0, 0, 0)));
		masspoints.push_back(MassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), Vec3(0, 0, 0), false, Vec3(0, 0, 0)));
		springs.push_back(Spring(0, 1, 1));
		break;
	case 2:
		masspoints.push_back(MassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), Vec3(0, 0, 0), false, Vec3(0, 0, 0)));
		masspoints.push_back(MassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), Vec3(0, 0, 0), false, Vec3(0, 0, 0)));
		springs.push_back(Spring(0, 1, 1));
		break;
	case 3:
		masspoints.push_back(MassPoint(Vec3(0, 2, 0), Vec3(-1, 0, 0), Vec3(0, 0, 0), false, Vec3(0, 0, 0)));
		masspoints.push_back(MassPoint(Vec3(0, 1, 0), Vec3(1, 0, 0), Vec3(0, 0, 0), false, Vec3(0, 0, 0)));
		masspoints.push_back(MassPoint(Vec3(-1, 0, 0), Vec3(1, 0, 0), Vec3(0, 0, 0), false, Vec3(0, 0, 0)));
		masspoints.push_back(MassPoint(Vec3(1, 0, 0), Vec3(1, 0, 0), Vec3(0, 0, 0), false, Vec3(0, 0, 0)));
		masspoints.push_back(MassPoint(Vec3(0, 0, 1), Vec3(1, 0, 0), Vec3(0, 0, 0), false, Vec3(0, 0, 0)));
		masspoints.push_back(MassPoint(Vec3(0, 0, -1), Vec3(1, 0, 0), Vec3(0, 0, 0), false, Vec3(0, 0, 0)));
		masspoints.push_back(MassPoint(Vec3(-1, 1, 0), Vec3(1, 0, 0), Vec3(0, 0, 0), false, Vec3(0, 0, 0)));
		masspoints.push_back(MassPoint(Vec3(1, -1, 0), Vec3(1, 0, 0), Vec3(0, 0, 0), false, Vec3(0, 0, 0)));
		masspoints.push_back(MassPoint(Vec3(0, 1, -1), Vec3(1, 0, 0), Vec3(0, 0, 0), false, Vec3(0, 0, 0)));
		masspoints.push_back(MassPoint(Vec3(0, -1, 1), Vec3(1, 0, 0), Vec3(0, 0, 0), false, Vec3(0, 0, 0)));

		springs.push_back(Spring(3, 0, 1));

		springs.push_back(Spring(3, 1, 1));

		springs.push_back(Spring(3, 4, 1));

		springs.push_back(Spring(3, 5, 1));

		springs.push_back(Spring(2, 0, 1));

		springs.push_back(Spring(2, 1, 1));

		springs.push_back(Spring(2, 4, 1));

		springs.push_back(Spring(2, 5, 1));

		springs.push_back(Spring(0, 1, 1));
		springs.push_back(Spring(1, 4, 1));
		springs.push_back(Spring(4, 5, 1));
		springs.push_back(Spring(5, 6, 1));
		springs.push_back(Spring(6, 7, 1));
		springs.push_back(Spring(7, 8, 1));
		springs.push_back(Spring(8, 9, 1));
		springs.push_back(Spring(9, 0, 1));

		break;
	default:
		break;
	}

}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass* DUC)
{
	this->DUC = DUC;
	switch (m_iIntegrator)
	{
	case 0:break;
	case 1:break;
	case 2:break;
	case 3: 
	{
		TwAddVarRW(DUC->g_pTweakBar, "Hold I Key to invert Velocity", TW_TYPE_BOOLCPP, &invertedVelocity, "");
		TwAddVarRW(DUC->g_pTweakBar, "Gravity", TW_TYPE_BOOLCPP, &isThereGravity, "");
		TwAddVarRW(DUC->g_pTweakBar, "TimeStep", TW_TYPE_FLOAT, &h, "min=0.000001");
		TwEnumVal shape[2] = { {0, "Euler"}, {1, "MidPoint"} };
		TwType shapeType = TwDefineEnum("", shape, 2);
		TwAddVarRW(DUC->g_pTweakBar, "Mode", shapeType, &currentCalculationMode, "");
		break;
	}

	default:break;
	}
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext) {

	Vec3 color{ 255, 64, 3 };
	Vec3 red{ 255, 0, 0 };
	float scale = 0.05;
	DUC->setUpLighting(Vec3(), 0.4 * Vec3(1, 1, 1), 100, 0.6 * Vec3(0.97, 0.86, 1));

	switch (m_iIntegrator) {
	case 0:
		DUC->drawSphere(masspoints.at(0).position, scale);
		DUC->drawSphere(masspoints.at(1).position, scale);

		DUC->beginLine();
		DUC->drawLine(masspoints.at(springs.at(0).point1).position, color, masspoints.at(springs.at(0).point2).position, color);
		DUC->endLine();
		break;
	case 1:
		DUC->drawSphere(masspoints.at(0).position, scale);
		DUC->drawSphere(masspoints.at(1).position, scale);

		DUC->beginLine();
		DUC->drawLine(masspoints.at(springs.at(0).point1).position, color, masspoints.at(springs.at(0).point2).position, color);
		DUC->endLine();
		break;
	case 2:
		DUC->drawSphere(masspoints.at(0).position, scale);
		DUC->drawSphere(masspoints.at(1).position, scale);

		DUC->beginLine();
		DUC->drawLine(masspoints.at(springs.at(0).point1).position, color, masspoints.at(springs.at(0).point2).position, color);
		DUC->endLine();
		break;
	case 3:

		for (size_t i = 0; i < masspoints.size(); i++) {

			DUC->drawSphere(masspoints.at(i).position, scale);

		}

		for (size_t i = 0; i < springs.size(); i++) {
			DUC->beginLine();
			DUC->drawLine(masspoints.at(springs.at(i).point1).position, color, masspoints.at(springs.at(i).point2).position, color);
			DUC->endLine();
		}
		
		DUC->beginLine();
		DUC->drawLine(Vec3(-2, 4, -2), red, Vec3(-2, 0, -2), red);
		DUC->endLine();

		DUC->beginLine();
		DUC->drawLine(Vec3(-2, 4, -2), red, Vec3(-2, 4, 2), red);
		DUC->endLine();

		DUC->beginLine();
		DUC->drawLine(Vec3(-2, 4, -2), red, Vec3(2, 4, -2), red);
		DUC->endLine();

		DUC->beginLine();
		DUC->drawLine(Vec3(-2, 4, 2), red, Vec3(2, 4, 2), red);
		DUC->endLine();

		DUC->beginLine();
		DUC->drawLine(Vec3(-2, 4, 2), red, Vec3(-2, 0, 2), red);
		DUC->endLine();

		DUC->beginLine();
		DUC->drawLine(Vec3(-2, 0, 2), red, Vec3(-2, 0, -2), red);
		DUC->endLine();

		DUC->beginLine();
		DUC->drawLine(Vec3(-2, 0, 2), red, Vec3(2, 0, 2), red);
		DUC->endLine();


		DUC->beginLine();
		DUC->drawLine(Vec3(2, 4, 2), red, Vec3(2, 4, -2), red);
		DUC->endLine();

		DUC->beginLine();
		DUC->drawLine(Vec3(2, 4, 2), red, Vec3(2, 0, 2), red);
		DUC->endLine();

		DUC->beginLine();
		DUC->drawLine(Vec3(2, 4, -2), red, Vec3(2, 0, -2), red);
		DUC->endLine();

		DUC->beginLine();
		DUC->drawLine(Vec3(2, 0, -2), red, Vec3(2, 0, 2), red);
		DUC->endLine();

		DUC->beginLine();
		DUC->drawLine(Vec3(2, 0, -2), red, Vec3(-2, 0, -2), red);
		DUC->endLine();
		
		break;
	}

	



}

void MassSpringSystemSimulator::notifyCaseChanged(int testCase)
{
	m_iIntegrator = testCase;
	switch (m_iIntegrator)
	{
	case 0:
		cout << "Basic Implementation !\n";
		reset();
		break;
	case 1:
		cout << "Euler !\n";
		reset();
		break;
	case 2:
		cout << "Midpoint !\n";
		reset();
		break;
	case 3:
		cout << "Simulation !\n";
		reset();
		break;
	default:
		cout << "Empty Test !\n";
		break;
	}
}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{

}

void MassSpringSystemSimulator::simulateTimestep(float timestep)
{
	Vec3 direction (0, 0, 0);
	Vec3 normalizedDirection (0, 0, 0);
	float lengthDirection = 0;
	Vec3 acceleration (0, 0, 0);
	Vec3 tempAcceleration (0, 0, 0);
	Vec3 position1(0, 0, 0);
	Vec3 position2(0, 0, 0);
	Vec3 position1half(0, 0, 0);
	Vec3 position2half(0, 0, 0);
	Vec3 midPointVelocity1(0, 0, 0);
	Vec3 midPointVelocity2(0, 0, 0);
	Vec3 midVelocityOfPoint(0, 0, 0);
	Vec3 underTheGround(0, 0, 0);

	// update current setup for each frame
	switch (m_iIntegrator)
	{

	case 0:
		// Basic Test just a Print out
		
		// Euler
		cout << "Euler: " << "\n";
		position1 = masspoints.at(0).position + 0.0 * masspoints.at(0).velocity;
		position2 = masspoints.at(1).position + 0.0 * masspoints.at(1).velocity;

		direction = position1 - position2;
		lengthDirection = sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
		normalizedDirection = direction / lengthDirection;
		masspoints.at(0).force = - m_fStiffness * (lengthDirection - springs.at(0).initialLength) * normalizedDirection;
		masspoints.at(1).force = - masspoints.at(0).force;

		acceleration = masspoints.at(0).force / m_fMass;

		masspoints.at(0).position = masspoints.at(0).position + 0.1 * masspoints.at(0).velocity;
		masspoints.at(1).position = masspoints.at(1).position + 0.1 * masspoints.at(1).velocity;

		cout << "Point 1 (t= 0.1)" << masspoints.at(0).position << "\n";
		cout << "Point 2 (t= 0.1)" << masspoints.at(1).position << "\n";
		cout << "\n";

		masspoints.at(0).velocity = masspoints.at(0).velocity + 0.1 * acceleration;
		masspoints.at(1).velocity = masspoints.at(1).velocity + 0.1 * -acceleration;

		cout << "Point 1 Velocity (t= 0.1) " << masspoints.at(0).velocity << "\n";
		cout << "Point 2 Velocity (t= 0.1)" << masspoints.at(1).velocity << "\n";
		
		//---------------------------------------------------------------------------------------------------------------------
		cout << "\n";
		cout << "Midpoint: " << "\n";
		
		// Midpoint
		
		// Midstep Position
		position1half = masspoints.at(0).position + 0.5 * 0.1 * masspoints.at(0).velocity;
		position2half = masspoints.at(1).position + 0.5 * 0.1 * masspoints.at(1).velocity;

		// Calculate normal Acceleration
		position1 = masspoints.at(0).position + 0.0 * masspoints.at(0).velocity;
		position2 = masspoints.at(1).position + 0.0 * masspoints.at(1).velocity;

		direction = position1 - position2;
		lengthDirection = sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
		normalizedDirection = direction / lengthDirection;

		masspoints.at(0).force = -m_fStiffness * (lengthDirection - springs.at(0).initialLength) * normalizedDirection;
		masspoints.at(1).force = -masspoints.at(0).force;

		acceleration = masspoints.at(0).force / m_fMass;

		// Midstep Velocity
		midPointVelocity1 = masspoints.at(0).velocity + 0.5 * 0.1 * acceleration;
		midPointVelocity2 = masspoints.at(1).velocity + 0.5 * 0.1 * -acceleration;

		// Step Position
		masspoints.at(0).position = masspoints.at(0).position + 0.1 * midPointVelocity1;
		masspoints.at(1).position = masspoints.at(1).position + 0.1 * midPointVelocity2;

		cout << "Point 1 (t= 0.1)" << position1 << "\n";
		cout << "Point 2 (t= 0.1)" << position2 << "\n";
		cout << "\n";

		// Midstep Acceleration

		direction = position1half - position2half;
		lengthDirection = sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
		normalizedDirection = direction / lengthDirection;

		masspoints.at(0).force = -m_fStiffness * (lengthDirection - springs.at(0).initialLength) * normalizedDirection;
		masspoints.at(1).force = -masspoints.at(0).force;


		acceleration = masspoints.at(0).force / m_fMass;

		// Step Velocity
		masspoints.at(0).velocity = masspoints.at(0).velocity + 0.1 * acceleration;
		masspoints.at(1).velocity = masspoints.at(1).velocity + 0.1 * -acceleration;

		cout << "Point 1 Velocity (t=0.1)" << masspoints.at(0).velocity << "\n";
		cout << "Point 2 Velocity (t=0.1)" << masspoints.at(1).velocity << "\n";
		cout << "\n";
		

		break;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
	case 1:

		// Euler
		cout << "Euler: " << "\n";

		position1 = masspoints.at(0).position + 0.0 * masspoints.at(0).velocity;
		position2 = masspoints.at(1).position + 0.0 * masspoints.at(1).velocity;

		direction = position1 - position2;
		lengthDirection = sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
		normalizedDirection = direction / lengthDirection;
		masspoints.at(0).force = -m_fStiffness * (lengthDirection - springs.at(0).initialLength) * normalizedDirection;
		masspoints.at(1).force = -masspoints.at(0).force;

		acceleration = masspoints.at(0).force / m_fMass;

		masspoints.at(0).position = masspoints.at(0).position + 0.005 * masspoints.at(0).velocity;
		masspoints.at(1).position = masspoints.at(1).position + 0.005 * masspoints.at(1).velocity;

		cout << "Point 1 (t= 0.005 )" << masspoints.at(0).position << "\n";
		cout << "Point 2 (t= 0.005)" << masspoints.at(1).position << "\n";
		cout << "\n";

		masspoints.at(0).velocity = masspoints.at(0).velocity + 0.005 * acceleration;
		masspoints.at(1).velocity = masspoints.at(1).velocity + 0.005 * -acceleration;

		cout << "Point 1 Velocity (t= 0.005) " << masspoints.at(0).velocity << "\n";
		cout << "Point 2 Velocity (t= 0.005)" << masspoints.at(1).velocity << "\n";
		cout << "\n";

		break;
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
	case 2:

		// Midpoint
		cout << "MidPoint: " << "\n";

		// Midstep Position
		position1half = masspoints.at(0).position + 0.5 * 0.005 * masspoints.at(0).velocity;
		position2half = masspoints.at(1).position + 0.5 * 0.005 * masspoints.at(1).velocity;

		// Calculate normal Acceleration
		position1 = masspoints.at(0).position + 0.0 * masspoints.at(0).velocity;
		position2 = masspoints.at(1).position + 0.0 * masspoints.at(1).velocity;

		direction = position1 - position2;
		lengthDirection = sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
		normalizedDirection = direction / lengthDirection;

		masspoints.at(0).force = -m_fStiffness * (lengthDirection - springs.at(0).initialLength) * normalizedDirection;
		masspoints.at(1).force = -masspoints.at(0).force;

		acceleration = masspoints.at(0).force / m_fMass;

		// Midstep Velocity
		midPointVelocity1 = masspoints.at(0).velocity + 0.5 * 0.005 * acceleration;
		midPointVelocity2 = masspoints.at(1).velocity + 0.5 * 0.005 * -acceleration;

		// Step Position
		masspoints.at(0).position = masspoints.at(0).position + 0.005 * midPointVelocity1;
		masspoints.at(1).position = masspoints.at(1).position + 0.005 * midPointVelocity2;

		cout << "Point 1 (t= 0.005)" << position1 << "\n";
		cout << "Point 2 (t= 0.005)" << position2 << "\n";
		cout <<   "\n";

		// Midstep Acceleration

		direction = position1half - position2half;
		lengthDirection = sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
		normalizedDirection = direction / lengthDirection;

		masspoints.at(0).force = -m_fStiffness * (lengthDirection - springs.at(0).initialLength) * normalizedDirection;
		masspoints.at(1).force = -masspoints.at(0).force;


		acceleration = masspoints.at(0).force / m_fMass;

		// Step Velocity
		masspoints.at(0).velocity = masspoints.at(0).velocity + 0.005 * acceleration;
		masspoints.at(1).velocity = masspoints.at(1).velocity + 0.005 * -acceleration;

		cout << "Point 1 Velocity (t= 0.005)" << masspoints.at(0).velocity << "\n";
		cout << "Point 2 Velocity (t= 0.005)" << masspoints.at(1).velocity << "\n";
		cout << "\n";
		break;
		
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------		
	case 3:
		// Simulation
		cout << "Simulation: "  << "\n";

		if (DXUTIsKeyDown('I')) {
			invertedVelocity = true;
		}
		else
		{
			invertedVelocity = false;
		}
		
		switch (currentCalculationMode)
		{

		case 0:
			// Euler
			cout << "Euler: " << "\n";
			cout << "\n";

			for each (Spring s in springs) {
				// Timestep 0

				position1 = getPositionOfMassPoint(s.point1) + h * getVelocityOfMassPoint(s.point1);
				position2 = getPositionOfMassPoint(s.point2) + h * getVelocityOfMassPoint(s.point2);

				// Timestep 1
				direction = position1 - position2;
				lengthDirection = sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
				normalizedDirection = direction / lengthDirection;
				masspoints.at(s.point1).force = -m_fStiffness * ( lengthDirection - s.initialLength ) * normalizedDirection;
				masspoints.at(s.point2).force = -masspoints.at(s.point1).force;
			}

			for (size_t i = 0; i < masspoints.size(); i++) {

				underTheGround = masspoints.at(i).position + h * masspoints.at(i).velocity;

				// Hits Floor y < 0
				if (underTheGround.y < 0)
				{
					if (acceleration.y < 0)
					{
						acceleration.y = - acceleration.y;
					}
					masspoints.at(i).position = masspoints.at(i).position + h * masspoints.at(i).velocity;
					masspoints.at(i).position.y = 0;
					
				}

				// Hits Ceiling y > 4
				if (underTheGround.y > 4)
				{
					if (acceleration.y > 4)
					{
						acceleration.y = - acceleration.y;
					}
					masspoints.at(i).position = masspoints.at(i).position + h * masspoints.at(i).velocity;
					masspoints.at(i).position.y = 4;

				}

				// Hits Left Side x < -2
				if (underTheGround.x < -2)
				{
					if (acceleration.x < -2)
					{
						acceleration.x = - acceleration.x;
					}
					masspoints.at(i).position = masspoints.at(i).position + h * masspoints.at(i).velocity;
					masspoints.at(i).position.x = -2;

				}

				// Hits Right Side x > 2
				if (underTheGround.x > 2)
				{
					if (acceleration.x > 2)
					{
						acceleration.x = - acceleration.x;
					}
					masspoints.at(i).position = masspoints.at(i).position + h * masspoints.at(i).velocity;
					masspoints.at(i).position.x = 2;

				}

				// Hits Front z < -2
				if (underTheGround.z < -2)
				{
					if (acceleration.z < -2)
					{
						acceleration.z = - acceleration.z;
					}
					masspoints.at(i).position = masspoints.at(i).position + h * masspoints.at(i).velocity;
					masspoints.at(i).position.z = -2;

				}

				// Hits Back z > 2
				if (underTheGround.z > 2)
				{
					if (acceleration.z > 2)
					{
						acceleration.z = - acceleration.z;
					}
					masspoints.at(i).position = masspoints.at(i).position + h * masspoints.at(i).velocity;
					masspoints.at(i).position.z = 2;

				}

				masspoints.at(i).position = masspoints.at(i).position + h * masspoints.at(i).velocity;
				cout << "Position of point " << i << " is: " << masspoints.at(i).position << "\n";

				// apply Gravity
				if (isThereGravity)
				{
					acceleration = masspoints.at(i).force / m_fMass + gravity;
				}
				else
				{
					acceleration = masspoints.at(i).force / m_fMass;
				}

				// Press Space to invert Velocity
				if (invertedVelocity) {
					invertedVelocity = true;
					for (int i = 0; i < masspoints.size(); i++)
					{
						masspoints.at(i).velocity = -(masspoints.at(i).velocity + h * acceleration);
					}
				}
				else 
				{
					masspoints.at(i).velocity = masspoints.at(i).velocity + h * acceleration;
				}
				cout << "Velocity of point " << i << " is: " << masspoints.at(i).velocity << "\n";

				cout << "\n";

			}

			break;

		case 1:
			cout << "MidPoint" << "\n";
			cout << "\n";

			// Midpoint

			for (size_t e = 0; e < springs.size(); e++) {
				// Timestep h

				position1 = getPositionOfMassPoint(springs.at(e).point1) + h * getVelocityOfMassPoint(springs.at(e).point1);
				position2 = getPositionOfMassPoint(springs.at(e).point2) + h * getVelocityOfMassPoint(springs.at(e).point2);

				direction = position1 - position2;
				lengthDirection = sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
				normalizedDirection = direction / lengthDirection;
				masspoints.at(springs.at(e).point1).force = -m_fStiffness * (lengthDirection - springs.at(e).initialLength) * normalizedDirection;
				masspoints.at(springs.at(e).point2).force = -masspoints.at(springs.at(e).point1).force;
			}

			for (size_t e = 0; e < springs.size(); e++) {
				// Acceleration at Timestep h/2

				position1half = getPositionOfMassPoint(springs.at(e).point1) + 0.5 * h * getVelocityOfMassPoint(springs.at(e).point1);
				position2half = getPositionOfMassPoint(springs.at(e).point2) + 0.5 * h * getVelocityOfMassPoint(springs.at(e).point2);

				direction = position1half - position2half;
				lengthDirection = sqrt((direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z));
				normalizedDirection = direction / lengthDirection;
				masspoints.at(springs.at(e).point1).tempForce = -m_fStiffness * (lengthDirection - springs.at(e).initialLength) * normalizedDirection;
				masspoints.at(springs.at(e).point2).tempForce = -masspoints.at(springs.at(e).point1).tempForce;

			}

			for (size_t i = 0; i < masspoints.size(); i++) {


	
				// Apply Gravity
				if (isThereGravity)
				{
					// Acceleration at Timestep h with gravity
					acceleration = masspoints.at(i).force / m_fMass + gravity;
				}
				else
				{
					// Acceleration at Timestep h
					acceleration = masspoints.at(i).force / m_fMass;
				}
		
				if (isThereGravity)
				{
					// Acceleration at Timestep h/2 with gravity
					tempAcceleration = masspoints.at(i).tempForce / m_fMass + gravity;
				}
				else
				{
					// Acceleration at Timestep h/2 with gravity
					tempAcceleration = masspoints.at(i).tempForce / m_fMass;
				}


				// Velocity at Timestep h/2
				midVelocityOfPoint = masspoints.at(i).velocity + 0.5 * h * acceleration;

				underTheGround = masspoints.at(i).position + h * midVelocityOfPoint;

				// Hits Floor y < 0
				if (underTheGround.y < 0)
				{
					if (tempAcceleration.y < 0)
					{
						tempAcceleration.y = -tempAcceleration.y;
					}
					masspoints.at(i).position = masspoints.at(i).position + h * midVelocityOfPoint;
					masspoints.at(i).position.y = 0;

				}

				// Hits Ceiling y > 4
				if (underTheGround.y > 4)
				{
					if (tempAcceleration.y > 4)
					{
						tempAcceleration.y = -tempAcceleration.y;
					}
					masspoints.at(i).position = masspoints.at(i).position + h * midVelocityOfPoint;
					masspoints.at(i).position.y = 4;

				}

				// Hits Left Side x < -2
				if (underTheGround.x < -2)
				{
					if (tempAcceleration.x < -2)
					{
						tempAcceleration.x = -tempAcceleration.x;
					}
					masspoints.at(i).position = masspoints.at(i).position + h * midVelocityOfPoint;
					masspoints.at(i).position.x = -2;

				}

				// Hits Right Side x > 2
				if (underTheGround.x > 2)
				{
					if (tempAcceleration.x > 2)
					{
						tempAcceleration.x = -tempAcceleration.x;
					}
					masspoints.at(i).position = masspoints.at(i).position + h * midVelocityOfPoint;
					masspoints.at(i).position.x = 2;

				}

				// Hits Front z < -2
				if (underTheGround.z < -2)
				{
					if (tempAcceleration.z < -2)
					{
						tempAcceleration.z = -tempAcceleration.z;
					}
					masspoints.at(i).position = masspoints.at(i).position + h * midVelocityOfPoint;
					masspoints.at(i).position.z = -2;

				}

				// Hits Back z > 2
				if (underTheGround.z > 2)
				{
					if (tempAcceleration.z > 2)
					{
						tempAcceleration.z = -tempAcceleration.z;
					}
					masspoints.at(i).position = masspoints.at(i).position + h * midVelocityOfPoint;
					masspoints.at(i).position.z = 2;

				}

				// Positon at Timestep h -> Final Position
				masspoints.at(i).position = masspoints.at(i).position + h * midVelocityOfPoint;

				cout << "Final Position of point " << i << " is: " << masspoints.at(i).position << "\n";

				// Press Space to invert Gravity
				if (invertedVelocity) {
					invertedVelocity = false;
					for (int i = 0; i < masspoints.size(); i++)
					{
						// Velocity at Timestep h -> Final Velocity inverted
						masspoints.at(i).velocity = -(masspoints.at(i).velocity + h * tempAcceleration);
					}
				}
				else
				{
					// Velocity at Timestep h -> Final Velocity
					masspoints.at(i).velocity = masspoints.at(i).velocity + h * tempAcceleration;
				}

				cout << "Final Velocity of point " << i << " is: " << masspoints.at(i).velocity << "\n";
				
			}
		break;
		}
		break;
	default:
		break;
	}
}

void MassSpringSystemSimulator::onClick(int x, int y) {
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void MassSpringSystemSimulator::onMouse(int x, int y) {
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void MassSpringSystemSimulator::setMass(float mass) {
	this->m_fMass = mass;
}

void MassSpringSystemSimulator::setStiffness(float stiffness) {
	this->m_fStiffness = stiffness;
}

void MassSpringSystemSimulator::setDampingFactor(float damping) {
	this->m_fDamping = damping;
}

int MassSpringSystemSimulator::addMassPoint(Vec3 position, Vec3 velocity, bool isFixed) {
	MassPoint newMasspoint = MassPoint(position, velocity, Vec3(0, 0, 0), isFixed, Vec3(0,0,0));
	masspoints.push_back(newMasspoint);
	return masspoints.size() - 1;
}

void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength) {
	Spring newSpring = Spring(masspoint1, masspoint2, initialLength);
	springs.push_back(newSpring);
}


int MassSpringSystemSimulator::getNumberOfMassPoints() {
	return masspoints.size();
}

int MassSpringSystemSimulator::getNumberOfSprings() {
	return springs.size();
}

Vec3 MassSpringSystemSimulator::getPositionOfMassPoint(int index) {
	return masspoints.at(index).position;
}

Vec3 MassSpringSystemSimulator::getVelocityOfMassPoint(int index) {
	return masspoints.at(index).velocity;
}

void MassSpringSystemSimulator::applyExternalForce(Vec3 force) {

}

