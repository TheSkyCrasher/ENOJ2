#include "animator.h"
#include <iostream>

Animator::Animator(const aiScene* scene, unsigned int animationIndex) {
	m_scene = scene;
	m_lastTime = 0.0;
	m_rootNode = NULL;
	m_currentAnimationIndex = -1;
	m_currentAnimation = NULL;
	m_lastFramePosition = NULL;
	m_identityMatrix = aiMatrix4x4();

	for (unsigned int i = 0; i < m_scene->mNumMeshes; i++) {
		const aiMesh* pCurrentMesh = m_scene->mMeshes[i];

		for (unsigned int j = 0; j < pCurrentMesh->mNumBones; j++) {
			const aiBone* pCurrentBone = pCurrentMesh->mBones[j];

			m_boneNodesByName[pCurrentBone->mName.data] = m_scene->mRootNode->FindNode(pCurrentBone->mName);
		}
	}

	m_lastSDLtime = 0;
	m_isPlaying = true;
	m_isPlayingForward = true;

	SetAnimationIndex(animationIndex);
}

Animator::~Animator() {
	if (m_rootNode) {
		delete m_rootNode;
	}

	if (m_lastFramePosition) {
		delete[] m_lastFramePosition;
	}
}

void Animator::SetAnimationIndex(unsigned int uAnimationIndex) {
	if (uAnimationIndex == m_currentAnimationIndex) {
		return;
	}

	if (m_rootNode != NULL) {
		delete m_rootNode;
		m_rootNode = NULL;
	}

	if (m_lastFramePosition != NULL) {
		delete[] m_lastFramePosition;
	}

	m_nodesByName.clear();
	m_currentAnimation = NULL;

	m_currentAnimationIndex = uAnimationIndex;
	m_currentAnimation = m_scene->mAnimations[m_currentAnimationIndex];

	m_rootNode = CreateNodeTree(m_scene->mRootNode, NULL);

	if (uAnimationIndex >= m_scene->mNumAnimations) {
		m_currentAnimationIndex = 0;
		m_currentAnimation = m_scene->mAnimations[m_currentAnimationIndex];
	}

	m_lastFramePosition = new Vector3i[m_currentAnimation->mNumChannels];
}

AnimationNode* Animator::CreateNodeTree(aiNode* pNode, AnimationNode * pParent) {
	AnimationNode* internalNode = new AnimationNode(pNode->mName.data);
	internalNode->parent = pParent;
	m_nodesByName[pNode] = internalNode;

	internalNode->localTransform = pNode->mTransformation;
	CalculateGlobalTransform(internalNode);

	if (m_currentAnimationIndex < m_scene->mNumAnimations) {
		internalNode->channelIndex = -1;

		for (unsigned int a = 0; a < m_currentAnimation->mNumChannels; a++) {
			if (m_currentAnimation->mChannels[a]->mNodeName.data == internalNode->name) {
				internalNode->channelIndex = a;

				break;
			}
		}
	}

	for (unsigned int a = 0; a < pNode->mNumChildren; a++) {
		AnimationNode* childNode = CreateNodeTree(pNode->mChildren[a], internalNode);
		internalNode->children.push_back(childNode);
	}

	return internalNode;
}

const std::vector<aiMatrix4x4>& Animator::GetBoneMatrices(const aiNode* node, unsigned int nodeMeshIndex) {
	if (nodeMeshIndex < node->mNumMeshes) {
		unsigned int uMeshIndex = node->mMeshes[nodeMeshIndex];

		if ((m_scene != NULL) && (uMeshIndex < m_scene->mNumMeshes)) {
			const aiMesh* pMesh = m_scene->mMeshes[uMeshIndex];

			for (unsigned int i = 0; i < m_transforms.size(); i++) {
				m_transforms[i] = aiMatrix4x4();
			}

			m_transforms.resize(pMesh->mNumBones, aiMatrix4x4());

			aiMatrix4x4 mGlobalInverseMeshTransform = GetGlobalTransform(node);
			mGlobalInverseMeshTransform.Inverse();

			for (unsigned int i = 0; i < pMesh->mNumBones; i++) {
				const aiBone* pBone = pMesh->mBones[i];
				const aiMatrix4x4& mCurrentGlobalTransform = GetGlobalTransform(m_boneNodesByName[pBone->mName.data]);
				m_transforms[i] = mGlobalInverseMeshTransform * mCurrentGlobalTransform * pBone->mOffsetMatrix;
			}
		}
	}

	return m_transforms;
}

void Animator::CalculateGlobalTransform(AnimationNode* pInternalNode) {
	pInternalNode->globalTransform = pInternalNode->localTransform;
	AnimationNode* pNode = pInternalNode->parent;

	while (pNode) {
		pInternalNode->globalTransform = pNode->localTransform * pInternalNode->globalTransform;
		pNode = pNode->parent;
	}
}

const aiMatrix4x4& Animator::GetGlobalTransform(const aiNode * node) const {
	NodeMap::const_iterator it = m_nodesByName.find(node);

	if (it == m_nodesByName.end()) {
		return m_identityMatrix;
	}

	return it->second->globalTransform;
}

void Animator::UpdateAnimation(double dTicksPerSecond) {
	long currentTime = SDL_GetTicks();
	long dt = currentTime - m_lastSDLtime;
	m_lastSDLtime = currentTime;

	if ((m_currentAnimation) && (m_currentAnimation->mDuration > 0.0 && m_isPlaying)) {
		if (m_isPlayingForward)
			m_elapsedTime += dt;
		else
			m_elapsedTime -= dt;

		double time = ((double)m_elapsedTime) / 1000.0;

		double ticksPerSecondCorrected = dTicksPerSecond != 0.0 ? dTicksPerSecond : 1.0;

		double timeInTicks = 0.0f;

		if (m_currentAnimation->mDuration > 0.0) {
			timeInTicks = fmod(time * ticksPerSecondCorrected, m_currentAnimation->mDuration);
		}

		if (m_transforms.size() != m_currentAnimation->mNumChannels) {
			m_transforms.resize(m_currentAnimation->mNumChannels);
		}

		for (unsigned int i = 0; i < m_currentAnimation->mNumChannels; i++) {
			const aiNodeAnim* pChannel = m_currentAnimation->mChannels[i];

			//******** Position *****
			aiVector3D vPresentPosition(0, 0, 0);

			if (pChannel->mNumPositionKeys > 0) {
				unsigned int frame = (timeInTicks >= m_lastTime) ? m_lastFramePosition[i].GetX() : 0;

				while (frame < pChannel->mNumPositionKeys - 1) {
					if (timeInTicks < pChannel->mPositionKeys[frame + 1].mTime) {
						break;
					}

					frame++;
				}

				unsigned int nextFrame = (frame + 1) % pChannel->mNumPositionKeys;
				const aiVectorKey& pKey = pChannel->mPositionKeys[frame];
				const aiVectorKey& pNextKey = pChannel->mPositionKeys[nextFrame];
				double dTimeDifference = pNextKey.mTime - pKey.mTime;

				if (dTimeDifference < 0.0) {
					dTimeDifference += m_currentAnimation->mDuration;
				}

				if (dTimeDifference > 0) {
					float fInterpolationFactor = (float)((timeInTicks - pKey.mTime) / dTimeDifference);
					vPresentPosition = pKey.mValue + (pNextKey.mValue - pKey.mValue) * fInterpolationFactor;
				}
				else {
					vPresentPosition = pKey.mValue;
				}

				m_lastFramePosition[i].SetX(frame);
			}

			//******** Rotation *********
			aiQuaternion presentRotation(1, 0, 0, 0);

			if (pChannel->mNumRotationKeys > 0) {
				unsigned int frame = (timeInTicks >= m_lastTime) ? m_lastFramePosition[i].GetY() : 0;

				while (frame < pChannel->mNumRotationKeys - 1) {
					if (timeInTicks < pChannel->mRotationKeys[frame + 1].mTime) {
						break;
					}

					frame++;
				}

				unsigned int nextFrame = (frame + 1) % pChannel->mNumRotationKeys;
				const aiQuatKey& pKey = pChannel->mRotationKeys[frame];
				const aiQuatKey& pNextKey = pChannel->mRotationKeys[nextFrame];
				double dTimeDifference = pNextKey.mTime - pKey.mTime;

				if (dTimeDifference < 0.0) {
					dTimeDifference += m_currentAnimation->mDuration;
				}

				if (dTimeDifference > 0) {
					float fInterpolationFactor = (float)((timeInTicks - pKey.mTime) / dTimeDifference);
					aiQuaternion::Interpolate(presentRotation, pKey.mValue, pNextKey.mValue, fInterpolationFactor);
				}
				else {
					presentRotation = pKey.mValue;
				}

				m_lastFramePosition[i].SetY(frame);
			}

			//******** Scaling **********
			aiVector3D vPresentScaling(1, 1, 1);

			if (pChannel->mNumScalingKeys > 0) {
				unsigned int frame = (timeInTicks >= m_lastTime) ? m_lastFramePosition[i].GetZ() : 0;

				while (frame < pChannel->mNumScalingKeys - 1) {
					if (timeInTicks < pChannel->mScalingKeys[frame + 1].mTime) {
						break;
					}

					frame++;
				}

				vPresentScaling = pChannel->mScalingKeys[frame].mValue;
				m_lastFramePosition[i].SetZ(frame);
			}

			aiMatrix4x4& mTransformation = m_transforms[i];
			mTransformation = aiMatrix4x4(presentRotation.GetMatrix());
			mTransformation.a1 *= vPresentScaling.x;
			mTransformation.a2 *= vPresentScaling.y;
			mTransformation.a3 *= vPresentScaling.z;
			mTransformation.a4 = vPresentPosition.x;

			mTransformation.b1 *= vPresentScaling.x;
			mTransformation.b2 *= vPresentScaling.y;
			mTransformation.b3 *= vPresentScaling.z;
			mTransformation.b4 = vPresentPosition.y;

			mTransformation.c1 *= vPresentScaling.x;
			mTransformation.c2 *= vPresentScaling.y;
			mTransformation.c3 *= vPresentScaling.z;
			mTransformation.c4 = vPresentPosition.z;
		}

		m_lastTime = timeInTicks;

		UpdateTransforms(m_rootNode, m_transforms);
	}
}

void Animator::UpdateTransforms(AnimationNode* node, const std::vector<aiMatrix4x4>& transforms){
	if (node->channelIndex != -1) {
		if (node->channelIndex >= transforms.size()) {
			return;
		}

		node->localTransform = transforms[node->channelIndex];
	}

	node->globalTransform = node->localTransform;
	AnimationNode* pParentNode = node->parent;

	while (pParentNode) {
		node->globalTransform = pParentNode->localTransform * node->globalTransform;
		pParentNode = pParentNode->parent;
	}

	for (std::vector<AnimationNode*>::iterator it = node->children.begin(); it != node->children.end(); it++) {
		UpdateTransforms(*it, transforms);
	}
}