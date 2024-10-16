#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <DirectXCollision.h>

namespace DX = DirectX;

template<typename T>
class QuadTree
{
public:
    QuadTree();
    QuadTree(const QuadTree& other);
    
public:
    
    void PrintTree() const;
    void AddElement(const std::shared_ptr<T>& elementAddress, const DX::BoundingBox& boundingBox);
    std::vector<std::shared_ptr<T>> CheckTree(const DX::BoundingFrustum& frustum) const; 

private:
    class Node
    {
    public:
        Node() = default;
        Node(const DX::BoundingBox& boundingBox, const int& depth)
        {
            this->ElementAddress = nullptr;
            this->ObjectBoundingBox = DX::BoundingBox();
            this->QuadrantBoundingBox = boundingBox;
            this->Depth = depth;
        }

        Node(const Node& other)
        {
            this->ElementAddress = other.ElementAddress;
            this->ObjectBoundingBox = other.ObjectBoundingBox;
            this->QuadrantBoundingBox = other.QuadrantBoundingBox;
            this->Depth = other.Depth;

            this->TopRight = std::make_unique<Node>(*other.TopRight);
            this->TopLeft = std::make_unique<Node>(*other.TopLeft);
            this->BottomLeft = std::make_unique<Node>(*other.BottomLeft);
            this->BottomRight = std::make_unique<Node>(*other.BottomRight);
        }
        
    public:
        bool IsLeafNode()
        {
            if
            (
                this->TopRight == nullptr &&
                this->TopLeft == nullptr &&
                this->BottomLeft == nullptr &&
                this->BottomRight == nullptr
            )
            {
                return true;
            }

            return false;
        }

        bool HasElement()
        {
            return this->ElementAddress != nullptr;
        }
        
    public:
        std::shared_ptr<T> ElementAddress = nullptr;
        DX::BoundingBox ObjectBoundingBox;
        
        DX::BoundingBox QuadrantBoundingBox;
        int Depth;
        
        std::unique_ptr<Node> TopRight = nullptr;
        std::unique_ptr<Node> TopLeft = nullptr;
        std::unique_ptr<Node> BottomLeft = nullptr;
        std::unique_ptr<Node> BottomRight = nullptr;
        
    };

private:
    void PrintTree(const std::unique_ptr<Node>& nodeToProcess) const;
    void AddToNode(const std::shared_ptr<T>& elementAddress, const DX::BoundingBox&, std::unique_ptr<Node>& node);
    void CheckNode(const DX::BoundingFrustum& frustum, const std::unique_ptr<Node>& node, std::vector<std::shared_ptr<T>>& foundObjects) const;
    
private:
    std::unique_ptr<Node> _root = nullptr;
    
};

template <typename T>
QuadTree<T>::QuadTree()
{
    DX::XMFLOAT3 center = {0.0f, 0.0f, 0.0f};
    DX::XMFLOAT3 extents = {50.0f, 50.0f, 50.0f};
    this->_root = std::make_unique<Node>(DX::BoundingBox(center, extents), 0);
}

template <typename T>
QuadTree<T>::QuadTree(const QuadTree& other)
{
    this->_root = std::make_unique<Node>(*other._root);
}



template <typename T>
void QuadTree<T>::PrintTree() const
{
    std::cout << "----- Start of Tree ----- \n";
    PrintTree(this->_root);
    std::cout << "----- End of Tree ----- " << std::endl;
}

template <typename T>
void QuadTree<T>::AddElement(const std::shared_ptr<T>& elementAddress, const DX::BoundingBox& boundingBox)
{
    AddToNode(elementAddress, boundingBox, this->_root);
}

template <typename T>
std::vector<std::shared_ptr<T>> QuadTree<T>::CheckTree(const DX::BoundingFrustum& frustum) const
{
    std::vector<std::shared_ptr<T>> toReturn;

    CheckNode(frustum, this->_root, toReturn);

    return toReturn;
}


template <typename T>
void QuadTree<T>::PrintTree(const std::unique_ptr<Node>& nodeToProcess) const
{
    std::cout << "Tree Level: " << nodeToProcess->Depth << ". Pointer: " << nodeToProcess->ElementAddress << ". \n";

    bool isLeafNode = true;

    if (nodeToProcess->TopRight != nullptr)
    {
        std::cout << "Top-Right" << "\n";
        PrintTree(nodeToProcess->TopRight);
        isLeafNode = false;
    }
    
    if (nodeToProcess->TopLeft != nullptr)
    {
        std::cout << "Top-Left" << "\n";
        PrintTree(nodeToProcess->TopLeft);
        isLeafNode = false;
    }
    
    if (nodeToProcess->BottomLeft != nullptr)
    {
        std::cout << "Bottom-Left" << "\n";
        PrintTree(nodeToProcess->BottomLeft);
        isLeafNode = false;
    }
    
    if (nodeToProcess->BottomRight != nullptr)
    {
        std::cout << "Bottom-Right" << "\n";
        PrintTree(nodeToProcess->BottomRight);
        isLeafNode = false;
    }

    if (isLeafNode == true)
    {
        std::cout << "This is a leaf node \n";
    }
}

template <typename T>
void QuadTree<T>::AddToNode(const std::shared_ptr<T>& elementAddress, const DX::BoundingBox& boundingBox, std::unique_ptr<Node>& node)
{
    bool collision = node->QuadrantBoundingBox.Contains(DX::XMLoadFloat3(&boundingBox.Center));

    if (!collision)
    {
        return;
    }

    if (node->IsLeafNode() == true)
    {
        if (node->HasElement() == false)
        {
            node->ElementAddress = elementAddress;
            node->ObjectBoundingBox = boundingBox;
            return;
        }
        else
        {
            if (node->Depth == 10)
            {
                return;
            }
            
            DX::XMFLOAT3 center = node->QuadrantBoundingBox.Center;
            DX::XMFLOAT3 extents = node->QuadrantBoundingBox.Extents;

            DX::XMFLOAT3 newExtents = {extents.x / 2, extents.y, extents.z / 2};
            DX::XMFLOAT3 topRightCenter = {center.x + newExtents.x, center.y, center.z + newExtents.z};
            DX::BoundingBox topRightQuadrant = DX::BoundingBox(topRightCenter, newExtents);
            
            node->TopRight = std::make_unique<Node>(topRightQuadrant, node->Depth + 1);

            
            DX::XMFLOAT3 topLeftCenter = {center.x - newExtents.x, center.y, center.z + newExtents.z};
            DX::BoundingBox topLeftQuadrant = DX::BoundingBox(topLeftCenter, newExtents);
            
            node->TopLeft = std::make_unique<Node>(topLeftQuadrant, node->Depth + 1);

            DX::XMFLOAT3 bottomLeftCenter = {center.x - newExtents.x, center.y, center.z - newExtents.z};
            DX::BoundingBox bottomLeftQuadrant = DX::BoundingBox(bottomLeftCenter, newExtents);
            
            node->BottomLeft = std::make_unique<Node>(bottomLeftQuadrant, node->Depth + 1);

            DX::XMFLOAT3 bottomRightCenter = {center.x + newExtents.x, center.y, center.z - newExtents.z};
            DX::BoundingBox bottomRightQuadrant = DX::BoundingBox(bottomRightCenter, newExtents);
            
            node->BottomRight = std::make_unique<Node>(bottomRightQuadrant, node->Depth + 1);

            AddToNode(node->ElementAddress, node->ObjectBoundingBox, node->TopRight);
            AddToNode(node->ElementAddress, node->ObjectBoundingBox, node->TopLeft);
            AddToNode(node->ElementAddress, node->ObjectBoundingBox, node->BottomLeft);
            AddToNode(node->ElementAddress, node->ObjectBoundingBox, node->BottomRight);

            node->ElementAddress = nullptr;
            node->ObjectBoundingBox = DX::BoundingBox();
        }
    }

    AddToNode(elementAddress, boundingBox, node->TopRight);
    AddToNode(elementAddress, boundingBox, node->TopLeft);
    AddToNode(elementAddress, boundingBox, node->BottomLeft);
    AddToNode(elementAddress, boundingBox, node->BottomRight);
}

template <typename T>
void QuadTree<T>::CheckNode(const DX::BoundingFrustum& frustum, const std::unique_ptr<Node>& node, std::vector<std::shared_ptr<T>>& foundObjects) const
{
    bool collision = frustum.Intersects(node->QuadrantBoundingBox);

    if (collision == false)
    {
        return;
    }

    if (node->IsLeafNode())
    {
        if (node->ElementAddress == nullptr)
        {
            return;
        }
        
        collision = frustum.Intersects(node->ObjectBoundingBox);

        if (collision)
        {
            auto iterator = std::find(foundObjects.begin(), foundObjects.end(), node->ElementAddress);

            if (iterator == foundObjects.end())
            {
                foundObjects.push_back(node->ElementAddress);
            }
        }
    }
    else
    {
        CheckNode(frustum, node->TopRight, foundObjects);
        CheckNode(frustum, node->TopLeft, foundObjects);
        CheckNode(frustum, node->BottomLeft, foundObjects);
        CheckNode(frustum, node->BottomRight, foundObjects);
    }
}
