pipeline {
    agent none

	parameters {
        string name: 'IMAGE', defaultValue: 'config-cpp-gcc831', description: 'Docker image'
		booleanParam name: 'RUN_TESTS', defaultValue: true, description: 'Run Tests?'
		booleanParam name: 'RUN_ANALYSIS', defaultValue: true, description: 'Run Static Code Analysis?'
	}
    stages {


        stage('build') {
            agent {
                docker {
                    label 'fir'
                    image "${params.IMAGE}:latest"
                }
            }
            steps {
                echo "building config-cpp branch ${env.BRANCH_NAME} using image ${params.IMAGE}"
                dir ("${params.IMAGE}") {
                    sh 'cmake -DYAML_SUPPORT=ON -DJSON_SUPPORT=ON -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$PWD/.. -DBUILD_TESTS=ON ..'
                    sh 'make'
                }
            }
        }

        stage('test') {
            agent {
                docker {
                    label 'fir'
                    image "${params.IMAGE}:latest"
                }
            }            
            when {
                environment name: 'RUN_TESTS', value: 'true'
            }
            steps {
                dir ("${params.IMAGE}") {
                    sh 'ctest --verbose'
                }
            }
        }

        stage ('analysis') {
            agent {
                docker {
                    label 'fir'
                    image "${params.IMAGE}:latest"
                }
            }
            when {
                environment name: "RUN_ANALYSIS", value: 'true'
            }
            steps {
                sh label: '', returnStatus: true, script: 'cppcheck . --xml --language=c++ --suppressions-list=suppressions.txt 2> cppcheck-result.xml'
                publishCppcheck allowNoReport: true, ignoreBlankFiles: true, pattern: '**/cppcheck-result.xml'
            }
        }
    }
    
}