pipeline {
    agent none
    stages {
        stage('gcc-8.3.1') {
            agent {
                docker {
                    label 'fir'
                    image 'config-cpp-gcc831:latest'
                    args '-u jenkins'
                }
            }
            steps {
                echo 'building config-cpp branch ${env.BRANCH_NAME} using gcc 8.3.1'
                dir ('gcc831') {
                    sh 'cmake -DYAML_SUPPORT=ON -DJSON_SUPPORT=ON -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$PWD/.. -DBUILD_TESTS=ON ..'
                    sh 'make'
                    sh 'ctest --verbose'
                }
            }

        }

        stage('gcc-9.1.0') {
            agent {
                docker {
                    label 'fir'
                    image 'config-cpp-gcc910:latest'
                    args '-u jenkins'
                }
            }
            steps {
                echo 'building config-cpp branch ${env.BRANCH_NAME} using gcc 9.1.0'
                dir ('gcc910') {
                    sh 'cmake -DYAML_SUPPORT=ON -DJSON_SUPPORT=ON -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$PWD/.. -DBUILD_TESTS=ON ..'
                    sh 'make'
                    sh 'ctest --verbose'
                }
            }

        }
    }
    
}