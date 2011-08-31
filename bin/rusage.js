/*
 * Copyright (C) 2011 by Jakub Lekstan <kuebzky@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

var rusage = require('./rusage.node'), labels = {}, defaultKey = "default";

exports.start = function(){
	var key, label, usage;

	if(arguments[0]){
		key = arguments[0];
	}else{
		key = defaultKey;
	}

	usage = rusage.cycles();

	labels[key] = usage;
	return labels[key];
};

exports.stop = function(){
	var key, usage;

	if(arguments[0]){
		key = arguments[0];
	}else{
		key = defaultKey;
	}

	usage = rusage.cycles();

	if(usage == null || labels[key] == null || !labels.hasOwnProperty(key)){
		return -1;
	}

	return usage - labels[key];
};

exports.usage = rusage.get;
exports.who = rusage.who;

//constants
exports.RUSAGE_SELF = rusage.RUSAGE_SELF;
exports.RUSAGE_CHILDREN = rusage.RUSAGE_CHILDREN;
