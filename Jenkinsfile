pipeline {
    agent none

	parameters {
        stringParam name: 'IMAGE', defaultValue: 'config-cpp-gcc831', description: 'Docker image'
		booleanParam name: 'RUN_TESTS', defaultValue: true, description: 'Run Tests?'
		booleanParam name: 'RUN_ANALYSIS', defaultValue: true, description: 'Run Static Code Analysis?'
	}
    stages {
        agent {
            docker {
                label 'fir'
                image '${params.IMAGE}:latest'
            }
        }

        stage('build') {
            steps {
                echo 'building config-cpp branch'
                dir ('${params.IMAGE}') {
                    sh 'cmake -DYAML_SUPPORT=ON -DJSON_SUPPORT=ON -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$PWD/.. -DBUILD_TESTS=ON ..'
                    sh 'make'
                }
            }
        }

        stage('test') {
            when {
                environment name: 'RUN_TESTS', value: 'true'
            }
            steps {
                dir ('${params.IMAGE}') {
                    sh 'ctest --verbose'
                }
            }
        }
    }
    
}