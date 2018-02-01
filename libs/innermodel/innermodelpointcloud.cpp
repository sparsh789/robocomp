/*
 * Copyright 2016 <copyright holder> <email>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

#include "innermodelpointcloud.h"

InnerModelPointCloud::InnerModelPointCloud(QString id_, InnerModelNode *parent_) : InnerModelNode(id_, parent_)
{
#if FCL_SUPPORT==1
	collisionObject = NULL;
#endif
	id = id_;
}

void InnerModelPointCloud::save(QTextStream &out, int tabs)
{
	Lock lock(mutex);
	for (int i=0; i<tabs; i++) out << "\t";
	out << "<pointcloud id=\""<<id<<"\"/>\n";
}

void InnerModelPointCloud::print(bool verbose)
{
	Lock lock(mutex);
	if (verbose) printf("Point Cloud: %s\n", qPrintable(id));
}

InnerModelNode * InnerModelPointCloud::copyNode(THash hash, InnerModelNode *parent)
{
	Lock lock(mutex);
	InnerModelPointCloud *ret = new InnerModelPointCloud(id, parent);
	ret->level = level;
	ret->fixed = fixed;
	ret->children->clear();
	ret->attributes.clear();
// 	hash->insert(id,ret);
// 
// 	for (QList<InnerModelNode*>::iterator i=children->begin(); i!=children->end(); i++)
// 	{
// 		ret->addChild((*i)->copyNode(hash, ret));
// 	}

	return ret;
}

