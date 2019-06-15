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
                cmake workingDir: 'gcc831', arguments: '-DYAML_SUPPORT=ON -DJSON_SUPPORT=ON -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$PWD/.. -DBUILD_TESTS=ON', installation: 'InSearchPath'
                cmakeBuild buildType: 'Debug', buildDir: 'gcc831', cleanBuild: true, installation: 'InSearchPath', steps: [[withCMake: true]]
                ctest workingDir: 'gcc831', arguments: '--verbose'               
            }

        }
    }
    
}