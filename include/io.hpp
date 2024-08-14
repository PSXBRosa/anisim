#pragma once
#include "generic.hpp"
#include "curves.hpp"
#include "shapes.hpp"

#include <iostream>
#include <fstream>
#include "rapidxml.hpp"

using namespace rapidxml;

void loadFromXML(std::string pathToXML){
  file<> xml(pathToXML);
  char* data = xml.data();

  xml_document<> doc;
  doc.parse(data);

  xml_node<>* root = doc.first_node("document");

  // parse variables
  xml_node<>* scene = root->first_node("scene");

  // link actions
  xml_node<>* events = root->first_node("events");
}
