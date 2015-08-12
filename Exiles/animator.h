#pragma once

#include "math.h"
#include "timer.h"

#include <map>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SDL2\SDL.h>

struct AnimationNode {
	AnimationNode() {
		channelIndex = -1;
		parent = NULL;
	}

	AnimationNode(const std::string& name) : name(name) {
		channelIndex = -1;
		parent = NULL;
	}

	~AnimationNode() {
		for (auto it = children.begin(); it != children.end(); ++it) {
			delete *it;
		}
	}

	std::string name;
	AnimationNode* parent;
	std::vector<AnimationNode*> children;

	aiMatrix4x4 localTransform;
	aiMatrix4x4 globalTransform;

	unsigned int channelIndex;
};

class Animator
{
public:
	Animator(const aiScene* scene, unsigned int animIndex);
	~Animator();

	void TogglePlaying() { m_isPlaying = !m_isPlaying; }

	void SetAnimationIndex(unsigned int animationIndex);
	void UpdateAnimation(double ticksPerSecond = 1.0);

	void PlayAnimationForward() { m_isPlayingForward = true; }
	void PlayAnimationBackward() { m_isPlayingForward = false; }

	const std::vector<aiMatrix4x4>& GetBoneMatrices(const aiNode* node, unsigned int meshIndex = 0);
	const aiMatrix4x4& GetGlobalTransform(const aiNode* node) const;

private:
	AnimationNode* CreateNodeTree(aiNode* node, AnimationNode* parent);
	void CalculateGlobalTransform(AnimationNode* internalNode);
	void UpdateTransforms(AnimationNode* node, const std::vector<aiMatrix4x4>& transforms);

	bool m_isPlaying;
	bool m_isPlayingForward;
	long m_elapsedTime;
	long m_lastSDLtime;

	typedef std::map<const aiNode*, AnimationNode*> NodeMap;
	NodeMap m_nodesByName;

	typedef std::map<const char*, const aiNode*> BoneMap;
	BoneMap m_boneNodesByName;

	double m_lastTime;
	Vector3i* m_lastFramePosition;

	std::vector<aiMatrix4x4> m_transforms;

	aiMatrix4x4 m_identityMatrix;

	const aiScene* m_scene;
	AnimationNode* m_rootNode;
	unsigned int m_currentAnimationIndex;
	const aiAnimation* m_currentAnimation;
};