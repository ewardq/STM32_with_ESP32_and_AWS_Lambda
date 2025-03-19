import json
import boto3
from boto3.dynamodb.conditions import Key


dynamodb = boto3.resource('dynamodb')
table = dynamodb.Table('DeviceData')

def lambda_handler(event, context):
    try:
        device_id = event['pathParameters']['device_id']
        timestamp = int(event['queryStringParameters']['timestamp'])

        response = table.delete_item(
            Key={
                'device_id': device_id,
                'timestamp': timestamp
            }
        )

        # Delete successful
        return {
            'statusCode': 200,
            'body': json.dumps({'message': 'Data deleted successfully'})
        }


    except KeyError as e:
        return {
            'statusCode': 400,
            'body': json.dumps({'error': 'Missing device_id parameter'})
        }

    except Exception as e:
        return {
            'statusCode': 500,
            'body': json.dumps({'error': str(e)})
        }
