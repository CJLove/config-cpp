pipeline {
    agent none

	parameters {
        booleanParam name: 'Use_gcc6', defaultValue: false, description: 'Build/test using gcc6'
        booleanParam name: 'Use_gcc7', defaultValue: false, description: 'Build/test using gcc7'
        booleanParam name: 'Use_gcc8', defaultValue: true, description: 'Build/test using gcc8'
        booleanParam name: 'Use_gcc9', defaultValue: true, description: 'Build/test using gcc9'
	}
    stages {
        stage('gcc8.3.1') {
            when {
                environment name: 'Use_gcc8', value: 'true'
            }
            agent {
                docker {
                    label 'fir'
                    image "config-cpp-gcc831:latest"
                }
            }
            steps {
                echo "building config-cpp branch ${env.BRANCH_NAME} using gcc 8.3.1"
                dir ("gcc831") {
                    sh 'cmake -DYAML_SUPPORT=ON -DJSON_SUPPORT=ON -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$PWD/.. -DBUILD_TESTS=ON ..'
                    sh 'make'
                    sh 'ctest -T test --no-compress-output'
                }
                
            }

            post {
                always {
                // Process the CTest xml output with the xUnit plugin
                xunit (
                    testTimeMargin: '3000',
                    thresholdMode: 1,
                    thresholds: [
                        skipped(failureThreshold: '0'),
                        failed(failureThreshold: '0')
                    ],
                    tools: [CTest(
                        pattern: 'gcc831/Testing/**/*.xml',
                        deleteOutputFiles: true,
                        failIfNotNew: false,
                    skipNoTestFiles: true,
                    stopProcessingIfError: true
                    )]
                )
                }
            }
        }
    }

        stage('gcc9.1.0') {
            when {
                environment name: 'Use_gcc9', value: 'true'
            }
            agent {
                docker {
                    label 'fir'
                    image "config-cpp-gcc910:latest"
                }
            }
            steps {
                echo "building config-cpp branch ${env.BRANCH_NAME} using gcc 9.1.0"
                dir ("gcc910") {
                    sh 'cmake -DYAML_SUPPORT=ON -DJSON_SUPPORT=ON -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$PWD/.. -DBUILD_TESTS=ON ..'
                    sh 'make'
                    sh 'ctest -T test --no-compress-output'
                }
                
            }

            post {
                always {
                // Process the CTest xml output with the xUnit plugin
                xunit (
                    testTimeMargin: '3000',
                    thresholdMode: 1,
                    thresholds: [
                        skipped(failureThreshold: '0'),
                        failed(failureThreshold: '0')
                    ],
                    tools: [CTest(
                        pattern: 'gcc910/Testing/**/*.xml',
                        deleteOutputFiles: true,
                        failIfNotNew: false,
                    skipNoTestFiles: true,
                    stopProcessingIfError: true
                    )]
                )
                }
            }
        }
    }
    
}