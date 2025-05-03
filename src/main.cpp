// SPDX-FileCopyrightText: 2025 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-only

#include "salalib/genlib/region4f.hpp"
#include "salalib/metagraphreadwrite.hpp"
#include "salalib/pointmap.hpp"
#include "salalib/shapegraph.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <iostream>
#include <vector>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

py::dict readMetaGraph(std::string fileName, bool verbose) {

  if (verbose) {
    std::cout << "Loading MetaGraph at: " << fileName << std::endl;
  }
  auto mgd = MetaGraphReadWrite::readFromFile(fileName);
  if (verbose) {
    std::cout << "- bb: " << mgd.metaGraph.region.area() << std::endl;
  }

  std::vector<py::dict> shapeMaps;
  std::vector<py::dict> shapeGraphs;
  std::vector<py::dict> pointMaps;

  for (auto &drawingFile : mgd.drawingFiles) {
    if (verbose) {
      std::cout << " - drawingName: " << drawingFile.first.name << std::endl;
    }
    for (auto it = drawingFile.second.begin(); it != drawingFile.second.end();
         ++it) {
      if (verbose) {
        std::cout << " - shapeMap name: " << it->getName() << std::endl;
      }
      py::dict mdata;
      mdata["group"] = "shape_" + drawingFile.first.name;
      mdata["map"] = std::move(*it);
      shapeMaps.emplace_back(std::move(mdata));
    }
  }
  for (auto it = mgd.dataMaps.begin(); it != mgd.dataMaps.end(); ++it) {
    if (verbose) {
      std::cout << " - dataMap name: " << it->getName() << std::endl;
    }
    py::dict mdata;
    mdata["group"] = "data";
    mdata["map"] = std::move(*it);
    shapeMaps.emplace_back(std::move(mdata));
  }
  for (auto it = mgd.shapeGraphs.begin(); it != mgd.shapeGraphs.end(); ++it) {
    if (verbose) {
      std::cout << " - dataMap name: " << it->getName() << std::endl;
    }
    std::string mapType = "none";
    switch (it->getMapType()) {
    case ShapeMap::AXIALMAP:
      mapType = "axial";
      break;
    case ShapeMap::ALLLINEMAP:
      mapType = "allline";
      break;
    case ShapeMap::SEGMENTMAP:
      mapType = "segment";
      break;
    case ShapeMap::CONVEXMAP:
      mapType = "convex";
      break;
    case ShapeMap::PESHMAP:
      mapType = "pesh";
      break;
    }
    py::dict mdata;
    mdata["type"] = mapType;
    mdata["map"] = std::move(*it);
    shapeGraphs.emplace_back(std::move(mdata));
  }
  for (auto it = mgd.pointMaps.begin(); it != mgd.pointMaps.end(); ++it) {
    if (verbose) {
      std::cout << " - dataMap name: " << it->getName() << std::endl;
    }
    py::dict mdata;
    mdata["map"] = std::move(*it);
    pointMaps.emplace_back(std::move(mdata));
  }
  py::dict maps;
  maps["shapeMaps"] = std::move(shapeMaps);
  maps["shapeGraphs"] = std::move(shapeGraphs);
  maps["pointMaps"] = std::move(pointMaps);

  return maps;
}

PYBIND11_MODULE(_core, m) {
  m.doc() = R"pbdoc(
        erodius: Spatial Network Analysis
        -----------------------

        .. currentmodule:: erodius

        .. autosummary::
           :toctree: _generate

           readMetaGraph
    )pbdoc";

  py::class_<ShapeMap>(m, "ShapeMap")            //
      .def(py::init<const std::string &, int>()) //
      .def("get_name", &ShapeMap::getName)       //
      // .def_readwrite("val", &A::val)
      ;

  py::class_<ShapeGraph>(m, "ShapeGraph") //
      .def(py::init<>())                  //
      ;

  py::class_<Point2f>(m, "Point2f")    //
      .def(py::init<double, double>()) //
      ;

  py::class_<Region4f>(m, "Region4f")    //
      .def(py::init<Point2f, Point2f>()) //
      ;

  py::class_<PointMap>(m, "PointMap")                 //
      .def(py::init<Region4f, const std::string &>()) //
      ;

  m.def("readMetaGraph", &readMetaGraph, R"pbdoc(
        Reads a metagraph

        Takes as input a filename and outputs a list of maps
    )pbdoc");

#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "dev";
#endif
}
