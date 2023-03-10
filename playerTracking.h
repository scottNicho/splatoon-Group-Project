#pragma once

#include"GameObject.h"
#include"Projectile.h"
#include<vector>
#include"Vector4.h"


namespace NCL::CSC8503 {

	class playerTracking :public GameObject {
	public:

		playerTracking();

		~playerTracking() {
			delete playerProjectile;
		}

		void setplayerID(int assignedPlayerID) {
			playerID = assignedPlayerID;
		}

		void setTeamID(int assignedTeamID) {
			playerID = assignedTeamID;
		}

		void resetPlayerProjectile() {
			playerProjectile = nullptr;
		}

		void setWeponType(gun newWeponType) {
			type = newWeponType;
		}

		void setPaintColor(Vector4 newPaintColor) {
			paintColor = newPaintColor;
		}

		void addToBulletsUsed(Projectile* bulletToAdd) {
			bulletsUsed.push_back(bulletToAdd);
		}

		void clearBulletsUsed();

		int getBulletVectorSize() {
			return bulletsUsed.size();
		}

		Vector4 getPaintColor() {
			return paintColor;
		}

		gun getWeponType() {
			return type;
		}

		/*void AssignPlayerWeapon(gun weponType) {
			playerProjectile->setGunType(weponType);
		}*/

		Projectile* getPlayerProjectile() {
			return playerProjectile;
		}
        

	protected:
		 
		 float playerYawOrientation;
		 float playerPitchOrientation;
		 int playerID;
		 int teamID;
		 int IndividualplayerScore;
		 Projectile* playerProjectile;
		 gun type;
		 Vector4 paintColor;
		 vector<Projectile*> bulletsUsed;
	};





}