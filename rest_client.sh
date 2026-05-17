#!/bin/bash

SERVER_URL="http://localhost:8080"

echo "Testing GET /items"
curl -s -X GET "$SERVER_URL/items" -H "Content-Type: application/json"
echo -e "\n"

echo "Testing POST /items (create item)"
curl -s -X POST "$SERVER_URL/items" -H "Content-Type: application/json" -d '{"name": "Item1", "description": "This is item 1", "price": 10.99}'
curl -s -X POST "$SERVER_URL/items" -H "Content-Type: application/json" -d '{"name": "Item2", "description": "This is item 2", "price": 20.99}'
echo -e "\n"

echo "Testing GET /items (fetch all items after creation)"
curl -s -X GET "$SERVER_URL/items" -H "Content-Type: application/json"
echo -e "\n"

echo "Testing GET /items/1 (fetch item with ID 1)"
curl -s -X GET "$SERVER_URL/items/1" -H "Content-Type: application/json"
echo -e "\n"

echo "Testing PUT /items/1 (update item with ID 1)"
curl -s -X PUT "$SERVER_URL/items/1" -H "Content-Type: application/json" -d '{"name": "Updated Item", "description": "Updated description", "price": 19.99}'
echo -e "\n"

echo "Testing GET /items/1 (fetch updated item with ID 1)"
curl -s -X GET "$SERVER_URL/items/1" -H "Content-Type: application/json"
echo -e "\n"

echo "Testing PATCH /items/1 (partially update item with ID 1)"
curl -s -X PATCH "$SERVER_URL/items/1" -H "Content-Type: application/json" -d '{"price": 15.49}'
echo -e "\n"

echo "Testing GET /items/1 (fetch partially updated item with ID 1)"
curl -s -X GET "$SERVER_URL/items/1" -H "Content-Type: application/json"
echo -e "\n"

echo "Testing DELETE /items/1 (delete item with ID 1)"
curl -s -X DELETE "$SERVER_URL/items/1" -H "Content-Type: application/json"
echo -e "\n"

echo "Testing GET /items (fetch all items after deletion)"
curl -s -X GET "$SERVER_URL/items" -H "Content-Type: application/json"
echo -e "\n"
