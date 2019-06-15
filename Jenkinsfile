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
                echo 'config-cpp using gcc 8.3.1'
                sh 'ls /home/jenkins/remote'
                sh 'cd /home/jenkins/remote/workspace/*'
                sh 'mkdir -p gcc831'
                sh 'cd gcc831'
                sh 'cmake -DYAML_SUPPORT=ON -DJSON_SUPPORT=ON -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$PWD/.. -DBUILD_TESTS=ON .. && make'
                sh 'make'
            }

        }
    }
    
}