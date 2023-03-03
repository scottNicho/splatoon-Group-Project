#pragma once
#include<iostream>
#include<string>
#include "GameObject.h"
#include<vector>
#include"Vector3.h"
#include<cmath>
#include<set>
#include"PhysicsObject.h"
#include <chrono>
#include <thread>
#include<windows.h>
#include <cstdlib>

using namespace std;

namespace NCL::CSC8503 {

	class EnemyAI :public GameObject {
	public:

		void preResponseCapture(GameObject* playerCap) {
			prePlayerPositionCapture = playerCap->GetTransform().GetPosition();
		}

		//void postResponse

		//Enemy Ai movement

		bool getEnemyDash() {
			return enemyDash;
		}

		void toggleEnemyDash() {
			enemyDash = !enemyDash;
		}


		void enemyMoveForward() {
			Vector3 enemyDirectionVector = this->GetTransform().GetOrientation() * Vector3 { 0, 0, -1 };
			this->GetPhysicsObject()->AddForce(enemyDirectionVector*20.0f);
		}

		void enemyMoveBackwards(GameObject* enemyAI) {
			Vector3 enemyDirectionVector = enemyAI->GetTransform().GetOrientation() * Vector3 { 0, 0, -1 };
			enemyAI->GetPhysicsObject()->AddForce(enemyDirectionVector * -10.0f);
		}

		void enemyMoveRight() {
			Vector3 enemyDirectionVector = this->GetTransform().GetOrientation() * Vector3 { 1, 0, 0 };
			this->GetPhysicsObject()->AddForce(enemyDirectionVector * 100.0f);
		}

		void enemyMoveLeft() {
			Vector3 enemyDirectionVector = this->GetTransform().GetOrientation() * Vector3 { -1, 0, 0 };
			this->GetPhysicsObject()->AddForce(enemyDirectionVector * 100.0f);
		}

		void enemyJump() {
			Vector3 enemyDirectionVector = this->GetTransform().GetOrientation() * Vector3 { 0, 1, 0 };
			this->GetPhysicsObject()->AddForce(enemyDirectionVector * 1100.0f);
		}

		void enemyCharge() {
			if (!(this->getEnemyDash())) {
				return;
			}
			Vector3 enemyDirectionVector = this->GetTransform().GetOrientation() * Vector3 { 0, 0, -1 };
			this->GetPhysicsObject()->AddForce(enemyDirectionVector * 5000.0f);
			this->toggleEnemyDash();
		}

		void enemyFaint() {
			Vector3 enemyDirectionVector = this->GetTransform().GetOrientation() * Vector3 { 0, 0, -1 };
			this->GetPhysicsObject()->AddForce(enemyDirectionVector * 5000.0f);
			Sleep(1000);
			this->GetPhysicsObject()->AddForce(enemyDirectionVector * -10000.0f);
		}

		void enemySideDodge() {
			int ranX = rand()%10;
			if (ranX >= 5) {
				enemyMoveRight();
				return;
			}
			enemyMoveLeft();
		}

		void moveToCentre() {
			Vector3 directionToCentre = (centreSafteyCircle - this->GetTransform().GetPosition()).Normalised();
			float distanceToCentre = directionToCentre.Length();
			this->GetPhysicsObject()->AddForce(directionToCentre * (10*distanceToCentre));
		}

		void updateEnemyAction() {
			enemyFaint();
		}

		//Enemy Ai movement 

		void UpdateDistanceFromCentre() {
			distanceFromCentre = (Vector3{0,-20,0} - this->GetTransform().GetPosition()).Length();
		}

		void togglePlayerClose() {
			playerClose = (!playerClose);
		}

		void setAggressionLevel(float newAgressionLevel) {
			agressionLevel = newAgressionLevel;
		}

		bool getFacingPlayer() {
			return facingPlayer;
		}

		void toggleFacingPlayer() {
			facingPlayer = !facingPlayer;
		}

		void setFacingPlayer(bool facing) {
			facingPlayer = facing;
		}

		bool getInsideAgressionRadius() {
			return insideAgressionRadius;
		}

		void updateInAgressionRadius() {
			if (distanceFromCentre < agressionRadius) {
				insideAgressionRadius =  true;
			}
			insideAgressionRadius = false;
		}

		float getAngleObjectTarget( GameObject* target) {
			Vector3 targetObjectPosition = target->GetTransform().GetPosition();
			Vector3 objectfacing = (this->GetTransform().GetOrientation() * Vector3 { 0, 0, -1 }).Normalised();
			Vector3 objectToTarget = ((targetObjectPosition) - (this->GetTransform().GetPosition())).Normalised();
			float dotProduct = Vector3::Dot(objectfacing, objectToTarget);
			float angleBetween = acos(dotProduct / ((objectfacing.Length()) * (objectToTarget.Length())));
			return angleBetween;
		}

		void faceTarget( GameObject* playerTarget) {
			Vector3 playerTargetPosition = playerTarget->GetTransform().GetPosition();
			float angle = getAngleObjectTarget(playerTarget);
			if (angle <= 0.2) {
				this->GetPhysicsObject()->ClearForces();
				this->GetPhysicsObject()->SetAngularVelocity({0,0,0});
				setFacingPlayer(true);
				return;
			}
			if (relativeLeftOrRight(playerTarget) == 1) {
				this->GetPhysicsObject()->AddTorque({ 0,1,0 });
			}
			else
			{
				this->GetPhysicsObject()->AddTorque({ 0,-1,0 });
			}

		}

		int relativeLeftOrRight(GameObject* targetPlayer) {
			Vector3 EnemyPosition = this->GetTransform().GetPosition();
			Vector3 TargetPosition = targetPlayer->GetTransform().GetPosition();
			//Vector2 EnemyRelativePosition = {  EnemyPosition.x, EnemyPosition.z}; //consider the enemy position as the origin
			Vector3 EnemyRelativeDirectionPosition = ((this->GetTransform().GetOrientation())*Vector3{0,0,-1});
			Vector2 EnemyRelativePosition = { EnemyRelativeDirectionPosition.x, EnemyRelativeDirectionPosition.z };
			float EnemyPositionGradient = EnemyRelativePosition.y / EnemyRelativePosition.x; // gradient of the cartiesian line going through the normalised direction vector and the origin
			Vector2 TargetRelativePosition = {TargetPosition.x - EnemyPosition.x,TargetPosition.z - EnemyPosition.z}; // the target augmented in to space relative to the enemy
			float interceptXPoint = (TargetRelativePosition.y * EnemyRelativePosition.x) / EnemyRelativePosition.y; // solve for the x value when a horisontal line from target intercepts
			Vector2 iterceptionPoint = {interceptXPoint,TargetRelativePosition.y };
			Vector2 targetToInterceptPoint = iterceptionPoint - TargetRelativePosition; // get the vector from target to intercep point to get the direction
			
			if (targetToInterceptPoint.x > 0 && (EnemyRelativePosition.y>= 0) ) {
				return -1;
			}
			if (targetToInterceptPoint.x > 0 && (EnemyRelativePosition.y < 0)) {
				return 1;
			}
			if (targetToInterceptPoint.x < 0 && (EnemyRelativePosition.y > 0)) {
				return 1;
			}

			if (targetToInterceptPoint.x < 0 && (EnemyRelativePosition.y < 0)) {
				return -1;
			}
			/*Vector3 EnemyDirectionVector = ((this->GetTransform().GetOrientation())*Vector3{0,0,-1}).Normalised();
			return EnemyDirectionVector;*/
			return 1;
		}


	protected:
		const  Vector3 centreSafteyCircle = Vector3{0,-20,0};
		Vector3 prePlayerPositionCapture = {};
		float distanceFromCentre = 0;
		float agressionRadius = 10;
		float agressionLevel = 0.5;
		const float innerCircleRadius = 8;
		const float outerCircleRadius = 15;
		bool playerClose = false;
		bool facingPlayer = false;
		bool enemyDash = true;
		bool insideAgressionRadius = false;
	};

}
