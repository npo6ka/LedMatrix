#pragma once

#include "snake_lib.h"

class AStarSnakeAI : public SnakeAI {
    struct Node {
        uint16_t dist;
        uint16_t cost;
        bool is_visited;
        bool is_locked;
        Trend trend;
    };

    // Queue<Trend, MAX_SNAKE_LENGTH>
    Node nodes[HEIGHT][WIDTH];
    Queue<Trend, MAX_SNAKE_LENGTH / 2> path;
    uint8_t amntStep = 0;
    static const uint8_t maxAmntStep = 1;

    Node &getNode(Coord coord) {
        return nodes[coord.x][coord.y];
    }

    int getOutCostFactor(Coord coord) {
        if (coord.x == 0 || coord.x == HEIGHT - 1 ||
            coord.y == 0 || coord.y == WIDTH - 1) {
            return 5;
        }
        return 0;
    }

    int getAtSnakeFactor(Coord coord, uint16_t cost) {
        for (auto i: {Trend::up, Trend::down, Trend::left, Trend::right}) {
            Coord val = coord.moveTo(i);
            if (val && getPix(val.x, val.y) == COLOR_SNAKE)
                return 30;
        }
        return 0;
    }

    void visitNode(Coord parent, Coord apple, Trend trend) {
        Coord coord = parent.moveTo(trend);
        if (!coord)
            return;

        Node &node = getNode(coord);

        if (node.is_locked)
            return;

        if (getPix(coord.x, coord.y) == COLOR_SNAKE) {
            node.is_locked = true;
            return;
        }

        uint16_t dist = getNode(parent).dist + 10;
        if (node.is_visited && dist >= node.dist) {
            return;
        }

        uint16_t distToApple = (abs(int(coord.x) - apple.x) + abs(int(coord.y) - apple.y)) * 10;

        node.trend = trend;
        node.dist = dist;
        node.cost = node.dist;
        node.cost += distToApple;
        node.cost += getOutCostFactor(coord);
        // node.cost += getAtSnakeFactor(coord, node.cost);
        node.is_visited = true;
    }

    void visitNeighbors(Coord coord, Coord apple) {
        visitNode(coord, apple, Trend::up);
        visitNode(coord, apple, Trend::down);
        visitNode(coord, apple, Trend::left);
        visitNode(coord, apple, Trend::right);

        getNode(coord).is_locked = true;
    }

    template <class Action>
    void printMas(Action &&action, const char *text) const {
        out("------------------- %s -----------------\n", text);
        for (uint8_t i = 0; i < HEIGHT; ++i) {
            for (uint8_t j = 0; j < WIDTH; ++j) {
                action(nodes[i][j]);
                out(" ");
            }
            out("\n");
        }
    }

    void debug(Coord head, Coord apple) const {
        out("head x: %d y: %d apple x: %d y: %d\n", head.x, head.y, apple.x, apple.y);

        printMas([](const Node &node) { out("%03d", node.cost); }, "cost");
        //printMas([](const Node &node) { out("%03d", node.dist); }, "dist");
        //printMas([](const Node &node) { out("%01d", (int)node.trend); }, "trend");
        //printMas([](const Node &node) { out("%01d", node.is_visited); }, "is_visited");
        //printMas([](const Node &node) { out("%01d", node.is_locked); }, "is_locked");

        path.debug();
    }

    Coord findMin() const {
        const Node *min = nullptr;
        Coord pos = {0xff, 0xff};
        for (uint8_t i = 0; i < HEIGHT; ++i) {
            for (uint8_t j = 0; j < WIDTH; ++j) {
                const Node &node = nodes[i][j];
                if (!node.is_locked && node.is_visited) {
                    if (!min || min->cost > node.cost) {
                        min = &node;
                        pos = {i, j};
                    }
                }
            }
        }
        return pos;
    }

    void calculatePath(Coord head, Coord apple) {
        clear();

        getNode(head).is_locked = true;
        getNode(head).dist = 0;

        visitNeighbors(head, apple);

        Coord pos;
        while((pos = findMin())) {
            if (pos == apple)
                break;
            visitNeighbors(pos, apple);
        }

        if (!pos) {
            return;
        }

        while (pos != head) {
            Trend vector = getNode(pos).trend;
            path.push(vector);
            pos.move(invertTrend(vector));
        }
    }

public:
    Trend getTrend(Coord head, Coord apple, Trend vector) override {
        amntStep++;
        if (!path.size() || amntStep == maxAmntStep) {
            calculatePath(head, apple);
            amntStep = 0;
        }

        if (!path.size()) {
            return Trend::none;
        }

        Trend trend = path.back();
        path.popBack();

        return trend;
    }

    void clear() override {
        memset(&nodes, 0, sizeof(nodes));
        path.clear();
    }
};