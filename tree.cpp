#include "tree.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace cv;
using namespace std;

Tree::Tree() = default;

Tree *Tree::compress(Mat *img, int threshold, int block) {
    return _compress(img, 0, 0, img->cols, img->rows, threshold * threshold * img->cols, block);
}

Mat Tree::decompress(Tree *tree, bool grid) {
    Mat output = Mat(tree->rows, tree->cols, CV_8UC3);
    _decompress(&output, tree, grid);
    return output;
}

Tree *Tree::_compress(Mat *img, int x, int y, int cols, int rows, int threshold, int block) {
    Tree *tree = new Tree();

    tree->x = x;
    tree->y = y;
    tree->cols = cols;
    tree->rows = rows;

    auto area = cols * rows;
    auto b = 0;
    auto g = 0;
    auto r = 0;

    for (auto i = y; i < y + cols; i++)
        for (auto j = x; j < x + rows; j++) {
            b += img->at<Vec3b>(i, j)[0];
            g += img->at<Vec3b>(i, j)[1];
            r += img->at<Vec3b>(i, j)[2];
        }

    b /= area;
    g /= area;
    r /= area;

    tree->rgb[0] = b;
    tree->rgb[1] = g;
    tree->rgb[2] = r;

    unsigned long long mean = 0;

    for (auto i = y; i < y + cols; i++)
        for (auto j = x; j < x + rows; j++) {
            auto mb = img->at<Vec3b>(i, j)[0];
            auto mg = img->at<Vec3b>(i, j)[1];
            auto mr = img->at<Vec3b>(i, j)[2];

            mean += (((b - mb) * (b - mb)) + ((g - mg) * (g - mg)) + ((r - mr) * (r - mr)) / (3 * area));
        }

    if (mean > threshold) {
        auto c = cols / 2;
        auto r = rows / 2;

        if (c >= block && r >= block) {
            tree->tl = _compress(img, x, y, c, r, threshold, block);
            tree->tr = _compress(img, x + r, y, c, r, threshold, block);
            tree->bl = _compress(img, x + r, y + c, c, r, threshold, block);
            tree->br = _compress(img, x, y + c, c, r, threshold, block);
        }
    }

    return tree;
}

void Tree::_decompress(Mat *img, Tree *tree, bool grid) {
    if (tree->tl == nullptr && tree->tr == nullptr && tree->bl == nullptr && tree->br == nullptr) {
        if (grid) {
            if (tree->x % 2 == 0) {
                tree->x += 1;
                tree->rows -= 1;
            }

            if (tree->y % 2 == 0) {
                tree->y += 1;
                tree->cols -= 1;
            }
        }

        for (auto i = tree->y; i < tree->y + tree->cols; i++)
            for (auto j = tree->x; j < tree->x + tree->rows; j++) {
                img->at<Vec3b>(i, j)[0] = tree->rgb[0];
                img->at<Vec3b>(i, j)[1] = tree->rgb[1];
                img->at<Vec3b>(i, j)[2] = tree->rgb[2];
            }
    } else {
        _decompress(img, tree->tl, grid);
        _decompress(img, tree->tr, grid);
        _decompress(img, tree->bl, grid);
        _decompress(img, tree->br, grid);
    }
}

void Tree::save(Tree *tree, FILE *file) {
    fwrite(tree, sizeof(*tree), 1, file);

    if (tree->tl != nullptr)
        save(tree->tl, file);

    if (tree->tr != nullptr)
        save(tree->tr, file);

    if (tree->bl != nullptr)
        save(tree->bl, file);

    if (tree->br != nullptr)
        save(tree->br, file);
}

void Tree::read(Tree *tree, FILE *file) {
    fread(tree, sizeof(*tree), 1, file);

    if (tree->tl != nullptr) {
        tree->tl = new Tree();
        read(tree->tl, file);
    }

    if (tree->tr != nullptr) {
        tree->tr = new Tree();
        read(tree->tr, file);
    }

    if (tree->bl != nullptr) {
        tree->bl = new Tree();
        read(tree->bl, file);
    }

    if (tree->br != nullptr) {
        tree->br = new Tree();
        read(tree->br, file);
    }
}