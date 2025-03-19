import json
import boto3
from boto3.dynamodb.conditions import Key
from decimal import Decimal

# Custom JSON encoder to handle Decimal types
class DecimalEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, Decimal):
            return float(obj)   # Convert Decimal to float for JSON serialization
        return super(DecimalEncoder, self).default(obj)


# Initialize DynamoDB resource and table
dynamodb = boto3.resource('dynamodb')
table = dynamodb.Table('DeviceData')

def lambda_handler(event, context):
    try:
        # Parse the query string paramenters
        device_id = event['queryStringParameters']['device_id']

        # Query DynamoDB for the latest item from the given device_id
        response = table.query(
            KeyConditionExpression=Key('device_id').eq(device_id),
            ScanIndexForward=False,     # Get the latest item (descending oerder)
            Limit=1                     # Return only the most recent item
        )

        if response['Items']:
            # Get the latest data
            latest_data = response['Items'][0]

            # Return the data with a 200 status code
            return {
                'statusCode': 200,
                'body': json.dumps(latest_data, cls=DecimalEncoder)   # Use custom encoder here
            }
        else: 
            # No data found for the given device_id
            return {
                'statusCode': 404,
                'body': json.dumps({'error': 'No data found for the given device_id'})
            }
    except KeyError as e:
        return {
            'statusCode': 400,
            'body': json.dumps({'error': f'Missing query parameter: {str(e)}'})
        }
    except Exception as e:
        return {
            'statusCode': 500,
            'body': json.dumps({'error': str(e)})
        }
