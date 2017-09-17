import json
import argparse
import os


global args, config

def getArgs():

	parser = argparse.ArgumentParser(description='Build,Deploy and Debug NDK projects.')
	parser.add_argument('-action', dest = 'action', required = True, default = 'clean', help = 'Type of action : Supported clean | build | build* | debug, where build* is build with NDK_DEBUG = 1')
	parser.add_argument('-project', dest = 'project', required = True, default = '.', help = 'Path of the project to build with ndk-build')
	parser.add_argument('-config', dest = 'config', required = True, default = 'sdk.json', help = 'Path of the config.json ')
	args = parser.parse_args()


def validateConfigJson(file):

	if os.path.isfile(file) == False:
		print "You did not pass a valid path for the config.json. Exiting \n"
		sys.exit(4)

	json = open(file, 'r')
	config = json.load(json)
	if not config['NDK']:
		print "NDK field is empty. Exiting \n"
		sys.exit(4)
	if not config['SDK']:
		print "SDK field is empty. Exiting \n"
		sys.exit(4)

	json.close()


def actionBuildNotOptimized():
	command = config['NDK'] + os.pathstep + 'ndk-build.cmd NDK_DEBUG=1'
	os.system(command)

def actionBuildOptimized():
	command = config['NDK'] + os.pathstep + 'ndk-build.cmd'
	os.system(command)

def actionClean():
	command = config['NDK'] + os.pathstep + 'ndk-build.cmd clean'
	os.system(command)

def actionDeploy():
	for f in listdir(config['TARGET']):
		file = join(config['TARGET'], f)
		command = config['SDK'] + os.pathstep + 'platform-tools' + os.pathstep + 'adb push ' + file + " " + config['ANDROID_DEPLOY_PATH'] 
		os.system(command)


options = {
			"clean" : actionClean,
			"build" : actionBuildNotOptimized,
			"build*" : actionBuildOptimized,
			"deploy" : actionDeploy
}


def executeAction(action):
	options[action]()



